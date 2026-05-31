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

pub struct Introduce_Mismatched_Generic_Argument_Count_In_Associated_Type_Binding_341;

impl Mutator for Introduce_Mismatched_Generic_Argument_Count_In_Associated_Type_Binding_341 {
    fn name(&self) -> &str {
        "Introduce_Mismatched_Generic_Argument_Count_In_Associated_Type_Binding_341"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MismatchedGenericArgumentVisitor::new();
        visitor.visit_file_mut(file);
        if let Some((target_path, target_args, new_args, target_segment, target_span)) = visitor.found_target {
            let mut new_file_string = file.to_token_stream().to_string();
            let old_args_str = target_args.to_token_stream().to_string();
            let new_args_str = new_args.to_token_stream().to_string();
            if let Some(pos) = new_file_string.find(&old_args_str) {
                new_file_string.replace_range(pos..pos + old_args_str.len(), &new_args_str);
                if let Ok(parsed) = syn::parse_file(&new_file_string) {
                    *file = parsed;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates an associated type binding within a trait object type inside an anonymous constant context, specifically targeting trait objects involving generic associated types (GATs) with lifetime parameters. It modifies the generic argument list for the associated type by either adding extra generic arguments (like lifetimes, types, or constants) or removing required ones, creating a mismatch in expected versus provided generic argument counts. The mutation reuses existing names from the seed program or introduces new lifetime parameters in the enclosing scope to ensure validity. This transformation aims to trigger the specific interaction between GATs, trait objects, and constant evaluation that caused the ICE in the bug report."
    }
}

struct MismatchedGenericArgumentVisitor {
    found_target: Option<(SynPath, PathArguments, PathArguments, PathSegment, Span)>,
    existing_idents: HashSet<String>,
    existing_lifetimes: HashSet<String>,
}

impl MismatchedGenericArgumentVisitor {
    fn new() -> Self {
        Self {
            found_target: None,
            existing_idents: HashSet::new(),
            existing_lifetimes: HashSet::new(),
        }
    }
}

impl VisitMut for MismatchedGenericArgumentVisitor {
    fn visit_file_mut(&mut self, file: &mut File) {
        self.collect_existing_names(file);
        if self.found_target.is_none() {
            self.visit_file_mut_inner(file);
        }
    }

    fn visit_item_mut(&mut self, item: &mut Item) {
        if self.found_target.is_nome() {
            match item {
                Item::Const(item_const) => {
                    if item_const.ident == "_" {
                        self.visit_block_mut(&mut item_const.block);
                    } else {
                        visit_mut::visit_item_mut(self, item);
                    }
                }
                _ => visit_mut::visit_item_mut(self, item),
            }
        }
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if self.found_target.is_nome() {
            if let Type::TraitObject(type_trait_object) = ty {
                for bound in &type_trait_object.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(colon2_token) = &trait_bound.path.leading_colon {
                            continue;
                        }
                        for segment in &mut trait_bound.path.segments {
                            if let PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                                for arg in &mut angle_args.args {
                                    if let GenericArgument::Binding(binding) = arg {
                                        if let PathArguments::AngleBracketed(assoc_args) = &mut binding.ty {
                                            let expected_count = if segment.ident.to_string().contains("Assoc") { 1 } else { 0 };
                                            let provided_count = assoc_args.args.len();
                                            if provided_count == expected_count {
                                                let mut new_args = assoc_args.clone();
                                                if expected_count == 1 {
                                                    if let Some(existing_type) = self.existing_idents.iter().next() {
                                                        let new_type_arg: GenericArgument = parse_quote!(#existing_type);
                                                        new_args.args.push(new_type_arg);
                                                    } else {
                                                        let new_type_arg: GenericArgument = parse_quote!(u32);
                                                        new_args.args.push(new_type_arg);
                                                    }
                                                } else {
                                                    let new_lifetime = if self.existing_lifetimes.is_empty() {
                                                        Lifetime::new("'a", Span::call_site())
                                                    } else {
                                                        Lifetime::new(&self.existing_lifetimes.iter().next().unwrap(), Span::call_site())
                                                    };
                                                    let new_lifetime_arg: GenericArgument = parse_quote!(#new_lifetime);
                                                    new_args.args.push(new_lifetime_arg);
                                                }
                                                self.found_target = Some((
                                                    trait_bound.path.clone(),
                                                    segment.arguments.clone(),
                                                    PathArguments::AngleBracketed(new_args),
                                                    segment.clone(),
                                                    ty.span(),
                                                ));
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_type_mut(self, ty);
    }
}

impl MismatchedGenericArgumentVisitor {
    fn collect_existing_names(&mut self, file: &File) {
        struct NameCollector<'a> {
            idents: &'a mut HashSet<String>,
            lifetimes: &'a mut HashSet<String>,
        }
        impl<'ast> Visit<'ast> for NameCollector<'ast> {
            fn visit_ident(&mut self, ident: &'ast Ident) {
                self.idents.insert(ident.to_string());
            }
            fn visit_lifetime(&mut self, lifetime: &'ast Lifetime) {
                self.lifetimes.insert(lifetime.to_string());
            }
        }
        let mut collector = NameCollector {
            idents: &mut self.existing_idents,
            lifetimes: &mut self.existing_lifetimes,
        };
        collector.visit_file(file);
    }

    fn visit_file_mut_inner(&mut self, file: &mut File) {
        for item in &mut file.items {
            self.visit_item_mut(item);
        }
    }
}