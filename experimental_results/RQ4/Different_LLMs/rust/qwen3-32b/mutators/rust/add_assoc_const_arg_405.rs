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

pub struct Add_Assoc_Const_Arg_405;

impl Mutator for Add_Assoc_Const_Arg_405 {
    fn name(&self) -> &str {
        "Add_Assoc_Const_Arg_405"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct AssocTypeConstVisitor;
        impl VisitMut for AssocTypeConstVisitor {
            fn visit_type_mut(&mut self, ty: &mut Type) {
                if let Type::TraitObject(trait_obj) = ty {
                    self.visit_type_trait_object_mut(trait_obj);
                }
                syn::visit_mut::visit_type_mut(self, ty);
            }

            fn visit_type_trait_object_mut(&mut self, trait_obj: &mut TypeTraitObject) {
                for bound in &mut trait_obj.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        let path = &mut trait_bound.path;
                        if let Some(segment) = path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in args.args.iter_mut() {
                                    if let GenericArgument::AssocType(assoc_type) = arg {
                                        let new_path = {
                                            let ident = assoc_type.ident.clone();
                                            let new_segment = PathSegment {
                                                ident,
                                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: punctuated::Punctuated::from_iter(vec![GenericArgument::Const(Expr::Verbatim(quote! { 1 }))]),
                                                    gt_token: token::Gt::default(),
                                                }),
                                            };
                                            let mut new_path = syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![new_segment]),
                                            };
                                            new_path
                                        };
                                        assoc_type.ty = Type::Path(TypePath {
                                            qself: None,
                                            path: new_path,
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_type_trait_object_mut(self, trait_obj);
            }

            fn visit_fn_arg_mut(&mut self, arg: &mut FnArg) {
                if let FnArg::Typed(pat_type) = arg {
                    self.visit_pat_type_mut(pat_type);
                }
                syn::visit_mut::visit_fn_arg_mut(self, arg);
            }

            fn visit_local_mut(&mut self, local: &mut Local) {
                self.visit_pat_mut(&mut local.pat);
                if let Some(ref mut init) = local.init {
                    self.visit_expr_mut(&mut init.expr);
                }
                syn::visit_mut::visit_local_mut(self, local);
            }
        }

        let mut visitor = AssocTypeConstVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms trait object associated type constraints by adding a generic const argument to the associated type's usage. This introduces const generics into existing trait bounds, potentially triggering type resolution errors, const evaluation mismatches, or invalid trait object constructions. It targets dynamic trait objects with associated types, ensuring maximal coverage of type constraint validation paths in the compiler."
    }
}