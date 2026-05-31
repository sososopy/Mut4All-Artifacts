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

pub struct Replace_Const_Generic_With_Type_In_Inherent_Assoc_417;

impl Mutator for Replace_Const_Generic_With_Type_In_Inherent_Assoc_417 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Type_In_Inherent_Assoc_417"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_type_needed = false;
        let mut dummy_type_added = false;
        
        let mut visitor = PathVisitor {
            dummy_type_needed: &mut dummy_type_needed,
        };
        visitor.visit_file_mut(file);
        
        if dummy_type_needed && !dummy_type_added {
            let dummy_struct = parse_quote! {
                struct DummyType;
            };
            file.items.insert(0, dummy_struct);
            dummy_type_added = true;
        }
        
        let mut replacer = PathReplacer {
            dummy_type_added,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets paths referencing inherent associated types with const generic arguments. It identifies such paths and replaces a const generic argument with a type that shares the same name but is not a const, mimicking the bug scenario where a type is mistakenly used where a const is expected. If no suitable type exists in scope, it introduces a dummy type at the module level. This transformation specifically triggers substitution errors in the compiler's const generic system when dealing with inherent associated types."
    }
}

struct PathVisitor<'a> {
    dummy_type_needed: &'a mut bool,
}

impl<'a> VisitMut for PathVisitor<'a> {
    fn visit_path_mut(&mut self, path: &mut SynPath) {
        if let Some(last_segment) = path.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Const(_) = arg {
                        *self.dummy_type_needed = true;
                        return;
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}

struct PathReplacer {
    dummy_type_added: bool,
}

impl VisitMut for PathReplacer {
    fn visit_path_mut(&mut self, path: &mut SynPath) {
        if let Some(last_segment) = path.segments.last_mut() {
            if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                let mut found_const = false;
                for arg in &mut args.args {
                    if let GenericArgument::Const(_) = arg {
                        found_const = true;
                        *arg = GenericArgument::Type(parse_quote! { DummyType });
                        break;
                    }
                }
                
                if found_const && !self.dummy_type_added {
                    let mut new_segments = Punctuated::new();
                    for segment in &path.segments {
                        new_segments.push(segment.clone());
                    }
                    *path = SynPath {
                        leading_colon: None,
                        segments: new_segments,
                    };
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}