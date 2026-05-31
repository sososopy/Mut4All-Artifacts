use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprClosure, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatIdent, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, TypeReference, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::{self, VisitMut},
    *,
};

use crate::mutator::Mutator;

pub struct Mutator_Closure_Return_Mutable_Ref_41;

impl Mutator for Mutator_Closure_Return_Mutable_Ref_41 {
    fn name(&self) -> &str {
        "Mutator_Closure_Return_Mutable_Ref_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms closures that capture a mutable reference to a collection (e.g., Vec, HashMap) into returning a reference to an element within that collection. This creates a borrow conflict by extending the lifetime of the mutable borrow through the returned reference, testing the compiler's borrowck system for closure return type handling and lifetime inference in complex borrow scenarios."
    }
}

struct ClosureVisitor;

impl VisitMut for ClosureVisitor {
    fn visit_expr_closure_mut(&mut self, node: &mut syn::ExprClosure) {
        if let Some(arg) = node.inputs.iter().next() {
            if let FnArg::Typed(ref pat_type) = *arg {
                if let Type::Reference(type_ref) = &*pat_type.ty {
                    if let Some(_) = type_ref.mutability {
                        if let Type::Path(type_path) = &*type_ref.elem {
                            let last_segment = type_path.path.segments.last().unwrap();
                            if last_segment.ident == "Vec" || last_segment.ident == "HashMap" {
                                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                    let param_name = &pat_ident.ident;
                                    let new_body = parse_quote! { & #param_name [ 0 ] }.unwrap();
                                    node.body = Some(Box::new(new_body));
                                }
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_closure_mut(self, node);
    }
}