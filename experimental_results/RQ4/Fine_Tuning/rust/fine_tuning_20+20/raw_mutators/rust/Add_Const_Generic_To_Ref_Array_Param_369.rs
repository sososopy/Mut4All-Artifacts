use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Add_Const_Generic_To_Ref_Array_Param_369;

impl Mutator for Add_Const_Generic_To_Ref_Array_Param_369 {
    fn name(&self) -> &str {
        "Add_Const_Generic_To_Ref_Array_Param_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut new_generics = item_fn.sig.generics.clone();
                let mut const_params = HashSet::new();
                for param in &new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_params.insert(const_param.ident.to_string());
                    }
                }
                let mut new_params = Vec::new();
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if let Type::Array(type_array) = &*type_ref.elem {
                                if let Expr::Path(expr_path) = &type_array.len {
                                    if expr_path.path.segments.len() == 1 {
                                        let seg = expr_path.path.segments.first().unwrap();
                                        if seg.ident != "usize" {
                                            let const_name = seg.ident.to_string();
                                            if !const_params.contains(&const_name) {
                                                new_params.push(const_name);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                for param in new_params {
                    new_generics.params.push(parse_quote!(const #param: usize));
                }
                item_fn.sig.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator identifies function parameters that are referenced arrays with a constant length, such as `&[[usize; N]; N]`, and adds the corresponding constant length identifiers as const generic parameters to the function signature. This transformation leverages Rust's const generics feature to increase the complexity of the function's type signature, potentially exposing compiler bugs related to const generic parameter handling and type inference."
    }
}