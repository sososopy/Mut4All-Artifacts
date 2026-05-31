pub(super) mod consts;
pub(super) mod utils;
pub(super) mod info;
pub(super) mod stmt;
pub(super) mod pat;
pub(super) mod expr;
pub(super) mod item;
pub(super) mod token;
pub(super) mod r#type;
pub(super) mod other;

use std::error::Error;

use info::env::ASTEnv;

#[allow(unused)]
pub trait FuzzerStrategyImpl<Node> {
    // fn do_mask(&self, _v: &mut dyn Fold, node: Node) -> Result<Node, Box<dyn Error>> {
    //     Ok(node)
    // }
    fn do_gen(&mut self) -> Result<Node, Box<dyn Error>>;
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64;
}

#[doc(hidden)]
#[allow(unused)]
pub enum DoFuzzRes<Node> {
    Success(Node),
    NoStreatgy(Node),
}

#[doc(hidden)]
pub struct FuzzerStrategyHolder<Node>
where
    Node: 'static,
{
    inner_ptr: *mut DynFuzzerStrategyTyper<Node>, // This is SUPER unsafe if wronly use it
}
unsafe impl<Node> Send for FuzzerStrategyHolder<Node> {}
unsafe impl<Node> Sync for FuzzerStrategyHolder<Node> {}
impl<Node> FuzzerStrategyHolder<Node> {
    pub fn new(b: Box<DynFuzzerStrategyTyper<Node>>) -> Self {
        let p = Box::leak(b);
        Self { inner_ptr: p }
    }
    pub fn inner(&self) -> &'static DynFuzzerStrategyTyper<Node> {
        unsafe { &*(self.inner_ptr as *const DynFuzzerStrategyTyper<Node>) }
    }
    pub fn inner_mut(&self) -> &'static mut DynFuzzerStrategyTyper<Node> {
        unsafe { &mut *self.inner_ptr }
    }
}

#[doc(hidden)]
// should impl by macro
pub trait FuzzerStrategyTyper<Node>: FuzzerStrategyImpl<Node> {
    fn is_kind(&self, node: &Node) -> bool;
    fn is_by_id(&self, id: &str) -> bool;
    fn get_id(&self) -> &str;
}

#[doc(hidden)]
pub type DynFuzzerStrategyTyper<Node> = dyn FuzzerStrategyTyper<Node> + Send + Sync + 'static;

#[macro_export]
macro_rules! register_nodetype {
    ($node_type: ident) => {
        #[allow(non_upper_case_globals)]
        #[allow(unused)]
        pub static ${concat("STRATEGY_HOLDER_", $node_type)}: std::sync::LazyLock<std::sync::RwLock<Vec<$crate::strategies::syn::strategy::FuzzerStrategyHolder<$node_type>>>> =
            std::sync::LazyLock::new(|| std::sync::RwLock::new(Vec::new()));
        #[allow(non_snake_case)]
        #[allow(unused)]
        pub fn ${concat("get_strategies_", $node_type)}() -> Result<std::sync::RwLockReadGuard<'static, Vec<$crate::strategies::syn::strategy::FuzzerStrategyHolder<$node_type>>>, Box<dyn Error>> {
            if let Ok(guard) = ${concat("STRATEGY_HOLDER_", $node_type)}.read() {
                Ok(guard)
            } else {
                return Err("Failed to lock strategy holder".into());
            }
        }
        // #[allow(non_snake_case)]
        // pub fn ${concat("do_mask_", $node_type)}(v: &mut dyn Fold, node: $node_type) -> Result<$crate::strategies::syn::strategy::DoFuzzRes<$node_type>, Box<dyn Error>> {
        //     let strategies = ${concat("get_strategies_", $node_type)}()?;
        //     for strategy in strategies.iter() {
        //         let inner = &strategy.inner();
        //         if !inner.is_kind(&node) {
        //             continue;
        //         }
        //         let res = inner.do_mask(v, node)?;
        //         return Ok($crate::strategies::syn::strategy::DoFuzzRes::Success(res));
        //     }
        //     Ok($crate::strategies::syn::strategy::DoFuzzRes::NoStreatgy(node))
        // }
        #[allow(non_snake_case)]
        #[allow(unused)]
        pub fn ${concat("do_gen_", $node_type)}(node: $node_type) -> Result<&'static mut $crate::strategies::syn::strategy::DynFuzzerStrategyTyper<$node_type>, Box<dyn Error>> {
            let strategies = ${concat("get_strategies_", $node_type)}()?;
            for strategy in strategies.iter() {
                let inner = &strategy.inner();
                if !inner.is_kind(&node) {
                    continue;
                }
                let inner = strategy.inner_mut();
                return Ok(inner);
            }
            Err(format!("No such a strategy by Gen {}: {:#?}", stringify!($node_type), node).into())
        }
        // #[allow(non_snake_case)]
        // pub fn ${concat("do_mask_id_", $node_type)}(v: &mut dyn Fold, id: &str, node: $node_type) -> Result<$crate::strategies::syn::strategy::DoFuzzRes<$node_type>, Box<dyn Error>> {
        //     let strategies = ${concat("get_strategies_", $node_type)}()?;
        //     for strategy in strategies.iter() {
        //         let inner = &strategy.inner();
        //         if !inner.is_by_id(id) {
        //             continue;
        //         }
        //         let res = inner.do_mask(v, node)?;
        //         return Ok($crate::strategies::syn::strategy::DoFuzzRes::Success(res));
        //     }
        //     Ok($crate::strategies::syn::strategy::DoFuzzRes::NoStreatgy(node))
        // }
        #[allow(non_snake_case)]
        #[allow(unused)]
        pub fn ${concat("do_gen_id_", $node_type)}(id: &str) -> Result<&'static mut $crate::strategies::syn::strategy::DynFuzzerStrategyTyper<$node_type>, Box<dyn Error>> {
            let strategies = ${concat("get_strategies_", $node_type)}()?;
            for strategy in strategies.iter() {
                let inner = &strategy.inner();
                if !inner.is_by_id(id) {
                    continue;
                }
                let inner = strategy.inner_mut();
                return Ok(inner);
            }
            Err(format!("No such a strategy by Id {}: {}", stringify!($node_type), id).into())
        }
        #[allow(non_snake_case)]
        #[allow(unused)]
        pub fn ${concat("get_all_id_", $node_type)}() -> Result<Vec<&'static $crate::strategies::syn::strategy::DynFuzzerStrategyTyper<$node_type>>, Box<dyn Error>> {
            let strategies = ${concat("get_strategies_", $node_type)}()?;
            let mut res = Vec::new();
            for strategy in strategies.iter() {
                let inner = strategy.inner();
                res.push(inner);
            }
            Ok(res)
        }
    };
}

#[macro_export]
macro_rules! use_nodetype {
    ($file: ident, $node_type: ident) => {
        #[allow(unused_imports)]
        use $crate::strategies::$file::${concat("STRATEGY_HOLDER_", $node_type)};
        #[allow(unused_imports)]
        use $crate::strategies::$file::${concat("get_strategies_", $node_type)};
        // #[allow(unused_imports)]
        // use $crate::fuzz::$file::${concat("do_mask_", $node_type)};
        #[allow(unused_imports)]
        use $crate::strategies::$file::${concat("do_gen_", $node_type)};
        // #[allow(unused_imports)]
        // use $crate::fuzz::$file::${concat("do_mask_id_", $node_type)};
        #[allow(unused_imports)]
        use $crate::strategies::$file::${concat("do_gen_id_", $node_type)};
        #[allow(unused_imports)]
        use $crate::strategies::$file::${concat("get_all_id_", $node_type)};
    };
}

#[macro_export]
macro_rules! do_mask {
    ($node_type: ident, $self: ident, $node: ident) => {
        // ${concat("do_mask_", $node_type)}($self, $node)
    };
}

#[macro_export]
macro_rules! do_mask_name {
    ($node_type: ident, $self: ident, $name: ident, $node: ident) => {
        ${concat("do_mask_id_", $node_type)}($self, stringify!(${concat("__strategy_id_", $name)}), $node)
    };
    ($file: ident, $node_type: ident, $self: ident, $name: ident, $node: ident) => {
        $crate::fuzz::$file::${concat("do_mask_id_", $node_type)}($self, stringify!(${concat("__strategy_id_", $name)}), $node)
    }
}

#[macro_export]
macro_rules! do_gen {
    ($node_type: ident, $node: ident) => {
        ${concat("do_gen_", $node_type)}($node)
    };
}

#[macro_export]
macro_rules! do_gen_name {
    ($node_type: ident) => {
        ${concat("do_gen_id_", $node_type)}(stringify!(${concat("__strategy_id_", $node_type, "Strategy")}))
    };
    ($node_type: ident, $name: ident) => {
        ${concat("do_gen_id_", $node_type)}(stringify!(${concat("__strategy_id_", $name)}))
    };
    ($file: ident, $node_type: ident, $name: ident) => {
        $crate::fuzz::$file::${concat("do_gen_id_", $node_type)}(stringify!(${concat("__strategy_id_", $name)}))
    }
}

#[macro_export]
macro_rules! do_gen_id {
    ($node_type: ident, $id: tt) => {
        ${concat("do_gen_id_", $node_type)}($id)
    };
    ($file: ident, $node_type: ident, $id: tt) => {
        $crate::fuzz::$file::${concat("do_gen_id_", $node_type)}($id)
    };
}

#[macro_export]
macro_rules! get_strategies {
    ($node_type: ident) => {
        ${concat("get_strategies_", $node_type)}()
    };
}

#[macro_export]
macro_rules! get_ids {
    ($node_type: ident) => {
        ${concat("get_all_id_", $node_type)}()
    };
}

#[macro_export]
macro_rules! register_strategy {
    ($name: ident, $node_type: ident, $node_kind: pat $(if $guard:expr)? $(,)?) => {
        impl $crate::strategies::syn::strategy::FuzzerStrategyTyper<$node_type> for $name {
            fn is_kind(&self, node: &$node_type) -> bool {
                matches!(node, $node_kind)
            }
            fn is_by_id(&self, id: &str) -> bool {
                id == stringify!(${concat("__strategy_id_", $name)})
            }
            fn get_id(&self) -> &str {
                stringify!(${concat("__strategy_id_", $name)})
            }
        }

        #[allow(non_snake_case)]
        #[cfg_attr(target_os="linux", ctor::ctor(link_section=".init_array.10"))]
        fn ${concat("__strategy_reg_ctor_", $name)}() {
            $crate::info!("Registered strategy: {}", stringify!($name));
            let b = Box::new($name::default());
            let strategy = $crate::strategies::syn::strategy::FuzzerStrategyHolder::new(b);
            if let Ok(mut guard) = ${concat("STRATEGY_HOLDER_", $node_type)}.write() {
                guard.push(strategy);
            }
        }
    };
}

#[macro_export]
macro_rules! register_strategy_no_kind {
    ($name: ident, $node_type: ident) => {
        impl $crate::strategies::syn::strategy::FuzzerStrategyTyper<$node_type> for $name {
            fn is_kind(&self, _: &$node_type) -> bool {
                false
            }
            fn is_by_id(&self, id: &str) -> bool {
                id == stringify!(${concat("__strategy_id_", $name)})
            }
            fn get_id(&self) -> &str {
                stringify!(${concat("__strategy_id_", $name)})
            }
        }

        #[allow(non_snake_case)]
        #[cfg_attr(target_os="linux", ctor::ctor(link_section=".init_array.10"))]
        fn ${concat("__strategy_reg_ctor_", $name)}() {
            $crate::info!("Registered strategy: {}", stringify!($name));
            let b = Box::new($name::default());
            let strategy = $crate::strategies::syn::strategy::FuzzerStrategyHolder::new(b);
            if let Ok(mut guard) = ${concat("STRATEGY_HOLDER_", $node_type)}.write() {
                guard.push(strategy);
            }
        }
    };
    ($name: ident, $node_type: ident, 50) => {
        impl $crate::strategies::syn::strategy::FuzzerStrategyTyper<$node_type> for $name {
            fn is_kind(&self, _: &$node_type) -> bool {
                false
            }
            fn is_by_id(&self, id: &str) -> bool {
                id == stringify!(${concat("__strategy_id_", $name)})
            }
            fn get_id(&self) -> &str {
                stringify!(${concat("__strategy_id_", $name)})
            }
        }

        #[allow(non_snake_case)]
        #[cfg_attr(target_os="linux", ctor::ctor(link_section=".init_array.50"))]
        fn ${concat("__strategy_reg_ctor_", $name)}() {
            $crate::info!("Registered strategy: {}", stringify!($name));
            let b = Box::new($name::default());
            let strategy = $crate::strategies::syn::strategy::FuzzerStrategyHolder::new(b);
            if let Ok(mut guard) = ${concat("STRATEGY_HOLDER_", $node_type)}.write() {
                guard.push(strategy);
            }
        }
    };
}