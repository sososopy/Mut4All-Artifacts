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

pub struct Replace_GAT_With_Concrete_In_HRTB_131;

impl Mutator for Replace_GAT_With_Concrete_In_HRTB_131 {
    fn name(&self) -> &str {
        "Replace_GAT_With_Concrete_In_HRTB_131"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = GatHrtbReplacer::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with Generic Associated Types (GATs) used within higher-ranked trait bounds (HRTBs). It identifies GAT references in function parameter types under HRTBs and replaces them with the concrete type defined in the same implementation, but only when the GAT is used directly and its definition is a simple type alias. This transformation exposes bugs in the compiler's handling of GATs in HRTB contexts, particularly when GATs are expanded inline instead of through associated type paths, potentially causing internal compiler errors."
    }
}

struct GatHrtbReplacer {
    gat_definitions: Vec<(String, Type)>,
}

impl GatHrtbReplacer {
    fn new() -> Self {
        Self {
            gat_definitions: Vec::new(),
        }
    }
}

impl VisitMut for GatHrtbReplacer {
    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        self.gat_definitions.clear();
        if let Some((_, _, path, _)) = &i.trait_ {
            if let Some(last_segment) = path.segments.last() {
                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                    for arg in &args.args {
                        if let GenericArgument::Type(_) = arg {
                        }
                    }
                }
            }
        }
        for item in &i.items {
            if let ImplItem::Type(assoc_type) = item {
                let gat_name = assoc_type.ident.to_string();
                if let Type::Path(type_path) = &assoc_type.ty {
                    if type_path.path.segments.len() == 1 {
                        let segment = &type_path.path.segments[0];
                        if segment.arguments.is_none() {
                            self.gat_definitions.push((gat_name, assoc_type.ty.clone()));
                        }
                    }
                }
            }
        }
        if !self.gat_definitions.is_empty() {
            for item in &mut i.items {
                if let ImplItem::Fn(method) = item {
                    self.visit_signature_mut(&mut method.sig);
                }
            }
        }
        self.gat_definitions.clear();
    }
    fn visit_signature_mut(&mut self, sig: &mut Signature) {
        for input in &mut sig.inputs {
            if let FnArg::Typed(PatType { ty, .. }) = input {
                self.visit_type_mut(ty);
            }
        }
    }
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::BareFn(bare_fn) => {
                if let Some(forall) = &bare_fn.lifetimes {
                    if forall.lifetimes.len() > 0 {
                        for input in &mut bare_fn.inputs {
                            self.visit_type_mut(&mut input.ty);
                        }
                    }
                }
            }
            Type::TraitObject(trait_obj) => {
                for bound in &mut trait_obj.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(lifetimes) = &trait_bound.lifetimes {
                            if lifetimes.lifetimes.len() > 0 {
                                self.visit_path_mut(&mut trait_bound.path);
                            }
                        }
                    }
                }
            }
            Type::Path(type_path) => {
                self.visit_path_mut(&mut type_path.path);
            }
            _ => {}
        }
    }
    fn visit_path_mut(&mut self, path: &mut Path) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Type(ty) = arg {
                        self.visit_type_mut(ty);
                    }
                }
            }
        }
        if path.segments.len() == \\
            let segment = &path.segments[0];
            if segment.arguments.is_none() {
                let gat_name = segment.ident.to_string();
                for (name, concrete_type) in &self.gat_definitions {
                    if &gat_name == name {
                        *ty = Type::Path(TypePath {
                            qself: None,
                            path: Path::from(Ident::new(&concrete_type.to_token_stream().to_string(), Span::call_site())),
                        });
                        break;
                    }
                }
            }
        }
    }
}