use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_With_Generic_Const_Parameter_206;

impl Mutator for Modify_Trait_With_Generic_Const_Parameter_206 {
    fn name(&self) -> &str {
        "Modify_Trait_With_Generic_Const_Parameter_206"
    }

    fn mutate(&self, file: &mut syn::File) {
        let feature_flag = parse_quote! {
            #![feature(generic_const_exprs)]
        };
        file.attrs.insert(0, feature_flag);
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Type(type_item) = item {
                        if let Some(const_param) = type_item.generics.const_params_mut().next() {
                            let mut rng = thread_rng();
                            let new_type = match rng.gen_range(0..3) {
                                0 => parse_quote!(u8),
                                1 => parse_quote!(i32),
                                _ => parse_quote!(u64),
                            };
                            const_param.ty = new_type;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait definitions with generic const parameters. It modifies the type of the const parameter to another integer type, such as changing from i16 to u8, i32, or u64. This change ensures that the rest of the code using the trait must adapt to the new type, potentially exposing type-related bugs."
    }
}