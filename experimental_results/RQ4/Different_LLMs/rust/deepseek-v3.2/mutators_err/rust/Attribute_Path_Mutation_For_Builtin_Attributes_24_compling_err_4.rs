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

pub struct Attribute_Path_Mutation_For_Builtin_Attributes_24;

impl Mutator for Attribute_Path_Mutation_For_Builtin_Attributes_24 {
    fn name(&self) -> &str {
        "Attribute_Path_Mutation_For_Builtin_Attributes_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        let builtin_attrs = vec!["derive", "cfg", "test", "inline", "repr", "should_panic", "allow", "deny", "warn", "forbid", "no_mangle", "must_use", "deprecated", "doc", "proc_macro", "proc_macro_attribute", "proc_macro_derive", "macro_use", "macro_export", "feature", "target_feature", "link_section", "cold", "optimize", "non_exhaustive", "automatically_derived", "panic_handler", "global_allocator", "windows_subsystem"];
        let dummy_names = vec!["skip", "attr", "dummy", "extra", "fake", "phantom", "shadow", "alias", "proxy", "wrapper"];
        
        let builtin_attrs_strings = builtin_attrs.iter().map(|s| s.to_string()).collect();
        let dummy_names_strings = dummy_names.iter().map(|s| s.to_string()).collect();
        
        let mut visitor = AttributePathMutatorVisitor {
            builtin_attrs: &builtin_attrs_strings,
            dummy_names: &dummy_names_strings,
        };
        visitor.visit_file_mut(file1);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets attribute paths in Rust code, specifically built-in attributes that are expected to be recognized by the compiler. It applies to any attribute usage in the seed program, including those on items, fields, statements, and expressions. The operator modifies the attribute path by inserting an extra path segment (using the double colon `::` syntax) after a built-in attribute name, creating a path that resembles a module-qualified attribute but where the initial segment is a built-in attribute name. This mimics the pattern in the bug report where `#[should_panic::skip]` was used, causing an internal compiler error because the attribute visitor did not handle the built-in attribute `should_panic` when it appeared as part of a longer path. This mutation is designed to expose bugs in the compiler's attribute checking logic, particularly where built-in attributes are not properly handled when they appear as part of a longer path. By mutating various built-in attributes similarly, we can test the robustness of the attribute processing code across different contexts and built-in names. This operator achieves high coverage by targeting all attribute usages, which are common in Rust code, and it adapts widely because it uses existing attribute names from the seed and introduces minimal new identifiers."
    }
}

struct AttributePathMutatorVisitor<'a> {
    builtin_attrs: &'a Vec<String>,
    dummy_names: &'a Vec<String>,
}

impl<'a> VisitMut for AttributePathMutatorVisitor<'a> {
    fn visit_attribute_mut(&mut self, attr: &mut syn::Attribute) {
        let path = attr.path();
        if path.segments.len() > 0 {
            let first_segment = &path.segments[0];
            let first_ident = first_segment.ident.to_string();
            if self.builtin_attrs.contains(&first_ident) {
                let mut rng = thread_rng();
                let dummy_name = self.dummy_names.choose(&mut rng).unwrap();
                let new_segment = syn::PathSegment {
                    ident: syn::Ident::new(dummy_name, Span::call_site()),
                    arguments: syn::PathArguments::None,
                };
                let mut new_segments = Punctuated::new();
                new_segments.push(first_segment.clone());
                new_segments.push(new_segment);
                attr.path = syn::Path {
                    leading_colon: None,
                    segments: new_segments,
                };
            }
        }
        syn::visit_mut::visit_attribute_mut(self, attr);
    }
}