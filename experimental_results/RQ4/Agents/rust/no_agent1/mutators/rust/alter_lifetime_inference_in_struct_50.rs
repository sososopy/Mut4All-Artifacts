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

pub struct Alter_Lifetime_Inference_In_Struct_50;

impl Mutator for Alter_Lifetime_Inference_In_Struct_50 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Inference_In_Struct_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut new_generics = item_struct.generics.clone();
                let lifetime_param = LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: Lifetime::new("'a", Span::call_site()),
                    colon_token: None,
                    bounds: Punctuated::new(),
                };
                new_generics.params.push(GenericParam::Lifetime(lifetime_param));
                
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &mut field.ty {
                        let segment = type_path.path.segments.last_mut().unwrap();
                        segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                            colon2_token: None,
                            lt_token: token::Lt::default(),
                            args: {
                                let mut args = Punctuated::new();
                                args.push(GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                                args
                            },
                            gt_token: token::Gt::default(),
                        });
                    }
                }
                item_struct.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a lifetime parameter to struct definitions and modifies field types to include this lifetime. This transformation stresses the compiler's lifetime inference and checking mechanisms, potentially leading to ICEs or other failures in scenarios where lifetime resolution becomes complex or ambiguous."
    }
}