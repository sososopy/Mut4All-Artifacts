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

pub struct Introduce_Trait_Object_Projection_In_Pattern_242;

impl Mutator for Introduce_Trait_Object_Projection_In_Pattern_242 {
    fn name(&self) -> &str {
        "Introduce_Trait_Object_Projection_In_Pattern_242"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ProjectionPatternVisitor {
            changed: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct patterns in match expressions where the struct contains fields with trait object projection types involving associated types that may not be sized. It transforms patterns like `S { .. }` into `S { f: x, .. }` with explicit bindings for projection fields, potentially exposing bugs in constructor set computation for unsized types in pattern matching contexts."
    }
}

struct ProjectionPatternVisitor {
    changed: bool,
}

impl VisitMut for ProjectionPatternVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        for arm in &mut expr.arms {
            if let Pat::Struct(pat_struct) = &mut arm.pat {
                if pat_struct.fields.is_empty() && pat_struct.rest.is_some() {
                    if let Some(item) = find_struct_item(&expr.arms, &pat_struct.path) {
                        if let Item::Struct(item_struct) = item {
                            for field in &item_struct.fields {
                                if is_trait_object_projection(&field.ty) {
                                    let field_name = field.ident.as_ref().unwrap();
                                    let binding_ident = Ident::new(&format!("projection_binding_{}", field_name), Span::call_site());
                                    let field_pat: Pat = parse_quote! { #field_name: #binding_ident };
                                    pat_struct.fields.push(FieldPat {
                                        attrs: Vec::new(),
                                        member: Member::Named(field_name.clone()),
                                        colon_token: Some(Default::default()),
                                        pat: Box::new(Pat::Ident(PatIdent {
                                            attrs: Vec::new(),
                                            by_ref: None,
                                            mutability: None,
                                            ident: binding_ident,
                                            subpat: None,
                                        })),
                                    });
                                    self.changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_match_mut(self, expr);
    }
}

fn find_struct_item<'a>(arms: &'a Vec<Arm>, path: &'a SynPath) -> Option<&'a Item> {
    for arm in arms {
        if let Pat::Struct(pat_struct) = &arm.pat {
            if pat_struct.path.segments.last().map(|s| &s.ident) == path.segments.last().map(|s| &s.ident) {
                return None;
            }
        }
    }
    None
}

fn is_trait_object_projection(ty: &Type) -> bool {
    if let Type::Path(type_path) = ty {
        if let Some(segment) = type_path.path.segments.last() {
            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                for arg in &args.args {
                    if let GenericArgument::Type(Type::TraitObject(trait_obj)) = arg {
                        if trait_obj.dyn_token.is_some() {
                            for bound in &trait_obj.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(projection_segment) = trait_bound.path.segments.last() {
                                        if projection_segment.ident == "Assoc" {
                                            return true;
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
    false
}