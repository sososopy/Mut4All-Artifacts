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

pub struct Adjust_Const_Generic_Params_For_Structs_342;

impl Mutator for Adjust_Const_Generic_Params_For_Structs_342 {
    fn name(&self) -> &str {
        "Adjust_Const_Generic_Params_For_Structs_342"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut used_names = HashSet::new();
                let mut new_params = Punctuated::new();

                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let new_name = generate_unique_name(&mut used_names);
                        used_names.insert(new_name.clone());

                        let new_const_param = ConstParam {
                            ident: Ident::new(&new_name, Span::call_site()),
                            colon_token: const_param.colon_token,
                            ty: const_param.ty.clone(),
                            eq_token: const_param.eq_token,
                            default: const_param.default.as_ref().map(|_| parse_quote! { 0 }),
                        };

                        new_params.push(GenericParam::Const(new_const_param));
                    } else {
                        new_params.push(param.clone());
                    }
                }

                item_struct.generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies struct definitions with const generic parameters and replaces these parameters with new names that are distinct and unique. Default values are replaced with constant expressions that do not reference other const parameters, preventing cyclic dependencies and ambiguities."
    }
}

fn generate_unique_name(used_names: &mut HashSet<String>) -> String {
    let mut rng = thread_rng();
    loop {
        let name = format!("A{}", rng.gen_range(0..1000));
        if !used_names.contains(&name) {
            return name;
        }
    }
}