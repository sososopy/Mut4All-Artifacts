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

pub struct Add_Const_Parameter_390;

impl Mutator for Add_Const_Parameter_390 {
    fn name(&self) -> &str {
        "Add_Const_Parameter_390"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    let new_generic_param: GenericParam = parse_quote! { const N: usize };
                    let mut new_inputs = item_fn.sig.inputs.clone();
                    let new_input: FnArg = parse_quote! { n: usize };
                    new_inputs.push(new_input);

                    let new_block: syn::Block = parse_quote! {{
                        n + 42
                    }};

                    item_fn.sig.generics.params.push(new_generic_param);
                    item_fn.sig.inputs = new_inputs;
                    item_fn.block = Box::new(new_block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function definitions with a simple return type. It introduces a new constant generic parameter and modifies the function to incorporate this parameter into its logic. This increases the complexity of the function signature and its internal computations, potentially exposing compiler bugs related to constant evaluation and type checking."
    }
}