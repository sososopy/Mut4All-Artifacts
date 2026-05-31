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



pub struct Replace_Pattern_Variant_With_Incompatible_Arity_244;



impl Mutator for Replace_Pattern_Variant_With_Incompatible_Arity_244 {

    fn name(&self) -> &str {

        "Replace_Pattern_Variant_With_Incompatible_Arity_244"

    }

    fn mutate(&self, file: &mut syn::File) {

        struct PatternVisitor {

            modified: bool,

        }



        impl VisitMut for PatternVisitor {

            fn visit_pat_mut(&mut self, i: &mut Pat) {

                if self.modified {

                    return;

                }



                if let Pat::TupleStruct(pat) = i {

                    if let Type::Path(type_path) = &*pat.base.ident.ty {

                        let path_segments = &type_path.path.segments;

                        if let [first] = path_segments.as_slice() {

                            let variant_name = &first.ident;

                            let new_variant = match variant_name.to_string().as_str() {

                                "Other1" => "Other2",

                                "Other2" => "Other1",

                                _ => return,

                            };



                            let new_variant_pat = Pat::TupleStruct(parse_quote! {

                                #type_path::#new_variant(foo, bar)

                            });



                            *i = new_variant_pat;

                            self.modified = true;

                            return;

                        }

                    }

                }



                visit_pat_mut(self, i);

            }

        }



        for item in &mut file.items {

            if let Item::Fn(func) = item {

                for stmt in &mut func.block.stmts {

                    if let Stmt::Local(local) = stmt {

                        let mut visitor = PatternVisitor { modified: false };

                        visitor.visit_local_mut(local);

                    }

                }

            }

        }

    }

    fn chain_of_thought(&self) -> &str {

        ""

    }

}