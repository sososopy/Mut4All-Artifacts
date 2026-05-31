use proc_macro2::Span;
use std::error::Error;
use syn::{
    AngleBracketedGenericArguments, GenericArgument, Ident, Path, PathArguments, PathSegment,
    Token, Type, TypePath, punctuated::Punctuated,
};

use crate::{
    do_gen_name,
    fuzz::rand_choose::WeightedRand,
    gen_token, register_strategy,
    strategies::syn::strategy::{info::env::ASTEnv, FuzzerStrategyImpl},
    use_nodetype,
};

use_nodetype!(syn, Type);

macro_rules! dty {
    ($t: tt) => {
        (Ident::new($t, Span::call_site()), 1)
    };
}

// Type Path only for default defined obj
pub struct TypePathStrategy {
    rnd: WeightedRand<Ident, u32>,
}
impl Default for TypePathStrategy {
    fn default() -> Self {
        let all_items = vec![
            dty!("i8"),
            dty!("i16"),
            dty!("i32"),
            dty!("i64"),
            dty!("u8"),
            dty!("u16"),
            dty!("u32"),
            dty!("u64"),
            dty!("str"),
            dty!("String"),
        ];
        Self {
            rnd: WeightedRand::new(Some(all_items), None),
        }
    }
}
unsafe impl Send for TypePathStrategy {}
unsafe impl Sync for TypePathStrategy {}
impl FuzzerStrategyImpl<Type> for TypePathStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let p = self.rnd.rand()?;
        let res = TypePath {
            qself: None,
            path: p.0.clone().into(),
        };
        Ok(Type::Path(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |_| 2.
    }
}

register_strategy!(TypePathStrategy, Type, Type::Path(..));

macro_rules! dtyg {
    ($($t: tt),* $(,)? ) => {
        dtyg!(@parse [] $($t)@ *)
    };
    (@fin $t: tt) => {
        PathSegment::from(
            Ident::new($t, Span::call_site())
        )
    };
    (@parse [ $($t: tt)* ]) => {
        (
            vec![
                $($t)*
            ].into_iter()
                .collect::<Punctuated<PathSegment, Token![::]>>(),
            1
        )
    };
    (@parse [] $a: literal) => {
        dtyg!(
            @parse [
                dtyg!(@fin $a)
            ]
        )
    };
    (@parse [ $($t: tt)* ] $a: literal) => {
        dtyg!(
            @parse [
                $($t)*
                ,dtyg!(@fin $a)
            ]
        )
    };
    (@parse [] $a: literal@ $($rest: tt)@*) => {
        dtyg!(
            @parse [
                dtyg!(@fin $a)
            ]
            $($rest)@*
        )
    };
    (@parse [ $($t: tt)* ] $a: literal@ $($rest: tt)@*) => {
        dtyg!(
            @parse [
                $($t)*
                ,dtyg!(@fin $a)
            ]
            $($rest)@*
        )
    };
}

// Type Path for types need generics
pub struct TypePathGenericStrategy {
    rnd: WeightedRand<Punctuated<PathSegment, Token![::]>, u32>,
}
impl Default for TypePathGenericStrategy {
    fn default() -> Self {
        let all_items = vec![
            dtyg!("Option"),
            dtyg!("Result"),
            dtyg!("std", "sync", "Arc"),
        ];
        Self {
            rnd: WeightedRand::new(Some(all_items), None),
        }
    }
}
unsafe impl Send for TypePathGenericStrategy {}
unsafe impl Sync for TypePathGenericStrategy {}
impl FuzzerStrategyImpl<Type> for TypePathGenericStrategy {
    fn do_gen(&mut self) -> Result<Type, Box<dyn Error>> {
        let p = self.rnd.rand()?;
        let mut seg = p.0.clone();
        let generic_arg = do_gen_name!(Type)?.do_gen()?;
        // For now, only one generic argument in type is given...
        // And also only type is considered...
        // todo: move to Generic when it is down
        let generic_arg = GenericArgument::Type(generic_arg);
        let mut generic_punc = Punctuated::default();
        generic_punc.push(generic_arg);
        let generic = AngleBracketedGenericArguments {
            colon2_token: None,
            lt_token: gen_token!(<),
            args: generic_punc,
            gt_token: gen_token!(>),
        };
        let generic = PathArguments::AngleBracketed(generic);
        seg.last_mut().ok_or("Unreachable?")?.arguments = generic;

        let res = TypePath {
            qself: None,
            path: Path {
                leading_colon: None,
                segments: seg,
            },
        };
        Ok(Type::Path(res))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 2. / (5 * e.type_nest) as f64
    }
}

register_strategy!(TypePathGenericStrategy, Type, Type::Path(..));
