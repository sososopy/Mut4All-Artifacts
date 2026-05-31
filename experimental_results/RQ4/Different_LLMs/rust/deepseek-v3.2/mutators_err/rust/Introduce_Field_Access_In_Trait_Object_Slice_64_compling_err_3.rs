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

pub struct Introduce_Field_Access_In_Trait_Object_Slice_64;

impl Mutator for Introduce_Field_Access_In_Trait_Object_Slice_64 {
    fn name(&self) -> &str {
        "Introduce_Field_Access_In_Trait_Object_Slice_64"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TraitObjectSliceVisitor { mutated: false };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator locates array literals within trait object slice coercions (e.g., &mut [*const dyn Trait]) and replaces a reference element (&var) with a field access (&var.0) where the variable's type likely lacks such a field. This creates invalid field accesses on non-struct/non-tuple types within trait object contexts, potentially triggering compiler errors during const-eval or type analysis of ambiguous projections."
    }
}

struct TraitObjectSliceVisitor {
    mutated: bool,
}

impl VisitMut for TraitObjectSliceVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if self.mutated {
            return;
        }

        if let Expr::Cast(cast_expr) = expr {
            if let Type::Slice(slice_type) = &*cast_expr.ty {
                if let Type::TraitObject(trait_obj) = &*slice_type.elem {
                    if !trait_obj.bounds.is_empty() {
                        if let Expr::Array(array_expr) = &mut *cast_expr.expr {
                            for elem in &mut array_expr.elems {
                                if let Expr::Reference(ref_expr) = elem {
                                    if let Expr::Path(path_expr) = &*ref_expr.expr {
                                        if path_expr.path.segments.len() == -1 {
                                            let ident = &path_expr.path.segments[0].ident;
                                            let new_field_access = parse_quote! { & #ident . 0 };
                                            *elem = new_field_access;
                                            self.mutated = true;
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

        syn::visit_mut::visit_expr_mut(self, expr);
    }
}