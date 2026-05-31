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

pub struct Modify_Static_Lifetime_Bound_48;

impl Mutator for Modify_Static_Lifetime_Bound_48 {
    fn name(&self) -> &str {
        "Modify_Static_Lifetime_Bound_48"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_static_lifetime = false;
                for field in &mut item_struct.fields {
                    if let Type::Reference(type_ref) = &mut field.ty {
                        if type_ref.lifetime.is_some() {
                            if type_ref.lifetime.as_ref().unwrap().ident == "static" {
                                has_static_lifetime = true;
                                type_ref.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                            }
                        }
                    }
                }
                if has_static_lifetime {
                    let lifetime_param = LifetimeParam::new(Lifetime::new("'a", Span::call_site()));
                    item_struct.generics.params.push(GenericParam::Lifetime(lifetime_param));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies struct definitions with fields having a static lifetime bound. It modifies these fields to use a non-static or generic lifetime, ensuring that the new lifetime parameter is declared in the struct definition. This transformation can uncover issues related to lifetime analysis and const evaluation by relaxing static guarantees and introducing more dynamic lifetime inference."
    }
}