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

pub struct Modify_Function_Return_Type_To_Const_Expr_191;

impl Mutator for Modify_Function_Return_Type_To_Const_Expr_191 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_To_Const_Expr_191"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut constants = Vec::new();

        for item in &file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    if type_path.path.is_ident("usize") {
                        constants.push(item_const.ident.clone());
                    }
                }
            }
        }

        if constants.is_empty() {
            constants.push(parse_quote!(NEW_CONSTANT));
            file.items.insert(0, parse_quote! {
                const NEW_CONSTANT: usize = 128;
            });
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ref mut ty) = item_fn.sig.output {
                    if !is_constant_expression(ty) {
                        let chosen_const = constants.choose(&mut rng).unwrap().clone();
                        *ty = Box::new(parse_quote!([usize; #chosen_const]));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator modifies the return type of functions to a constant expression, specifically a constant-sized array. It identifies functions with non-constant return types and changes them to use a constant from the existing constants in the code. If no suitable constant is available, it introduces a new constant. This tests the Rust compiler's handling of constant evaluation."
    }
}

fn is_constant_expression(ty: &Type) -> bool {
    if let Type::Array(_) = ty {
        true
    } else {
        false
    }
}