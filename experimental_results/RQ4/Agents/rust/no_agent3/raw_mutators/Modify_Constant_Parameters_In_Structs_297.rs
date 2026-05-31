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

pub struct Modify_Constant_Parameters_In_Structs_297;

impl Mutator for Modify_Constant_Parameters_In_Structs_297 {
    fn name(&self) -> &str {
        "Modify_Constant_Parameters_In_Structs_297"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_static_lifetime = false;
                for generic_param in item_struct.generics.params.iter_mut() {
                    if let GenericParam::Const(const_param) = generic_param {
                        if let Type::Reference(type_ref) = &*const_param.ty {
                            if type_ref.lifetime.as_ref().map_or(false, |lt| lt.ident == "static") {
                                has_static_lifetime = true;
                                const_param.ty = Box::new(Type::Reference(TypeReference {
                                    lifetime: Some(Lifetime::new("'a", Span::call_site())),
                                    ..type_ref.clone()
                                }));
                            }
                        }
                    }
                }
                if has_static_lifetime {
                    item_struct.generics.params.insert(0, GenericParam::Lifetime(LifetimeDef {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'a", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with constant parameters having a 'static lifetime. It replaces the 'static lifetime with a generic lifetime 'a, updating the struct's generic parameters and types accordingly. This modification tests the Rust compiler's handling of lifetime parameters in constant generic contexts."
    }
}