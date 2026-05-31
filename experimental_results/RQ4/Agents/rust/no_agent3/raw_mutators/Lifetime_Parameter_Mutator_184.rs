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

pub struct Lifetime_Parameter_Mutator_184;

impl Mutator for Lifetime_Parameter_Mutator_184 {
    fn name(&self) -> &str {
        "Lifetime_Parameter_Mutator_184"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_lifetime = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Lifetime(_) = param {
                        has_lifetime = true;
                        break;
                    }
                }

                if has_lifetime {
                    for param in &mut item_struct.generics.params {
                        if let GenericParam::Lifetime(lifetime) = param {
                            lifetime.lifetime = Lifetime::new("'static", Span::call_site());
                            break;
                        }
                    }
                } else {
                    item_struct.generics.params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: vec![],
                        lifetime: Lifetime::new("'static", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));

                    if let Some(field) = item_struct.fields.iter_mut().next() {
                        if let Type::Reference(ref mut type_ref) = field.ty {
                            type_ref.lifetime = Some(Lifetime::new("'static", Span::call_site()));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct declarations with lifetime parameters. It modifies an existing lifetime parameter to use the reserved name `'static`. If no lifetime parameter exists, it introduces a new one with the name `'static` and ensures it is used in a field type of the struct."
    }
}