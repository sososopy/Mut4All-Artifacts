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

pub struct Modify_ProcMacro_Param_Type_477;

impl Mutator for Modify_ProcMacro_Param_Type_477 {
    fn name(&self) -> &str {
        "Modify_ProcMacro_Param_Type_477"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut custom_type_declared = false;

        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if item_fn.attrs.iter().any(|attr| attr.path.is_ident("proc_macro_attribute")) {
                    if !custom_type_declared {
                        let custom_type: ItemStruct = parse_quote! {
                            struct MyCustomType;
                        };
                        file.items.insert(0, Item::Struct(custom_type));
                        custom_type_declared = true;
                    }

                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.is_ident("TokenStream") {
                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: parse_quote!(MyCustomType),
                                    }));
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions annotated with `#[proc_macro_attribute]`. It modifies one of the `TokenStream` parameters to a custom type `MyCustomType`, which is declared as an empty struct at the start of the module. This transformation challenges the compiler's handling of procedural macros by introducing unexpected types, potentially leading to macro hygiene issues or token processing errors."
    }
}