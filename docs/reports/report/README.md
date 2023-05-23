<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!-- PROJECT LOGO -->
<br />
<div align="center">

<h1 align="center">PPII2</h1>

  <p align="center">
    project_description
    <br />
    <a href="https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07/-/tree/main/docs"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07">View Demo</a>
    ·
    <a href="https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07/issues">Report Bug</a>
    ·
    <a href="https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

PoinCarWash is a project designed to provide an innovative solution to one of the major challenges faced by electric vehicle owners: finding charging stations when on the road. The project incorporates advanced algorithms to recommend a list of charging stations for a given vehicle range, ensuring that EV owners can plan their journeys more efficiently without worrying about running out of charge.

In addition to its powerful recommendation engine, the PoinCarWash project also includes a chargers network monitoring function that gives a real-time view of charger usage rates. This data is invaluable for service providers and network operators, helping them to optimize their operations and improve the overall experience for EV drivers.

The project is built on open-source technology, making it accessible to the broader EV community. The source code is available on GitLab, allowing developers to contribute to the project and improve its functionality in meaningful ways. Our aim is to create a community-driven solution to the challenge of EV charging, helping to promote the widespread adoption of electric vehicles and reduce the carbon footprint of transportation.

We welcome anyone interested in this project to join the community and contribute to its growth and development. Together, we can create a sustainable future for transportation and the planet at large.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

- ![C][c]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

The Makefile is designed to build with Clang and cURL so make sure to have it installed

- llvm
  ```sh
  clang --version
  ```
- curl
  ```sh
  apt-get install -yq clang libcurl4-openssl-dev
  ldconfig
  ```

### Installation

1. Clone the repo
   ```sh
   git clone https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07.git
   ```
2. Build all
   ```sh
   cd src
   make all
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->

## Usage

### Plan a trip

```sh
./trip <start_location> <end_location> <vehicle_name>
```

**Example:**

```sh
./trip "3 rue stanislas nancy" "toulouse" "Tesla Model 3"
```

### Network monitoring

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->

## Roadmap

- [x] Plan a journey
  - [x] CLI input
  - [x] API address to coordinates conversion
- [ ] Network monitoring

See the [open issues](https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->

## Contact

- Stanislas MEZUREUX <<stanislas.mezureux@telecomnancy.eu>>
- Corentin BILLARD <<corentin.billard@telecomnancy.eu>>
- Yann DIONISIO <<yann.dionisio@telecomnancy.eu>>
- Antonin FREY <<antonin.frey@telecomnancy.eu>>

Project Link: [https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07](https://gitlab.telecomnancy.univ-lorraine.fr/ppii2_2223/ppii2-grp_07)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->

## Acknowledgments

- [Etalab](https://www.data.gouv.fr/fr/datasets/fichier-consolide-des-bornes-de-recharge-pour-vehicules-electriques/)
- [EVD](https://ev-database.org/cheatsheet/range-electric-car)
- [Snow](https://github.com/mortie/snow)
- [OpenStreetMap](https://www.openstreetmap.org/)
- [Google Maps](https://www.google.com/maps)
- [cURL](https://curl.se/)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[c]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
