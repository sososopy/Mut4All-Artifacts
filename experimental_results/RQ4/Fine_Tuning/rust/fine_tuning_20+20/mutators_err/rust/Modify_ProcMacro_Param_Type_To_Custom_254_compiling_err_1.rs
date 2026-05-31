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

pub struct Modify_ProcMacro_Param_Type_To_Custom_254;

impl Mutator for Modify_ProcMacro_Param_Type_To_Custom_254 {
    fn name(&self) -> &str {
        "Modify_ProcMacro_Param_Type_To_Custom_254"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_custom_type = false;
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "MyCustomType" {
                    has_custom_type = true;
                    break;
                }
            }
        }
        if !has_custom_type {
            file.items.insert(0, parse_quote! { struct MyCustomType; });
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn
                    .attrs
                    .iter()
                    .any(|attr| attr.path.is_ident("proc_macro_attribute"))
                {
                    for input in &mut item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path
                                    .path
                                    .segments
                                    .last()
                                    .map_or(false, |segment| segment.ident == "TokenStream")
                                {
                                    pat_type.ty = Box::new(parse_quote! { MyCustomType });
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
        "This mutator targets functions annotated with #[proc_macro_attribute] and modifies one of their TokenStream parameters to use a custom type, MyCustomType, which is declared as an empty struct if not already present. This transformation aims to disrupt the expected procedural macro parameter type, potentially triggering compiler errors related to macro expansion, token processing, or type resolution."
    }
}