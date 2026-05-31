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

pub struct Add_Recursive_Type_Alias_485;

impl Mutator for Add_Recursive_Type_Alias_485 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_485"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let type_alias_name = Ident::new("RecursiveType", Span::call_site());

        let recursive_type_alias: Item = parse_quote! {
            type #type_alias_name = Box<#type_alias_name>;
        };

        file.items.push(recursive_type_alias);

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let param_name = Ident::new("recursive_param", Span::call_site());
                    let param_type: Type = parse_quote!(#type_alias_name);

                    let new_param = FnArg::Typed(PatType {
                        attrs: Vec::new(),
                        pat: Box::new(Pat::Ident(PatIdent {
                            attrs: Vec::new(),
                            by_ref: None,
                            mutability: None,
                            ident: param_name,
                            subpat: None,
                        })),
                        colon_token: token::Colon::default(),
                        ty: Box::new(param_type),
                    });

                    func.sig.inputs.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveType = Box<RecursiveType>;` and adds it as a parameter to non-main functions. This transformation creates a deeply recursive type, potentially leading to infinite recursion during type resolution, stack overflow, or other compiler resource exhaustion issues. It aims to test the robustness of the compiler's type handling mechanisms."
    }
}