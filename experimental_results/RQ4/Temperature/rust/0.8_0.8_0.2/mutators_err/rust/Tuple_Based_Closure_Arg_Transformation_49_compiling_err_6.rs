use syn::{parse_quote, PathSegment, AngleBracketedGenericArguments, visit_mut, Type, TypePath, Ident, punctuated::Punctuated, PathArguments, token::{Lt, Gt}, GenericArgument, ExprCall, Expr, visit_mut::VisitMut};
use quote::quote;
use crate::mutator::Mutator;

pub struct Tuple_Based_Closure_Arg_Transformation_49;

impl Mutator for Tuple_Based_Closure_Arg_Transformation_49 {
    fn name(&self) -> &str {
        "Tuple_Based_Closure_Arg_Transformation_49"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                for input in &mut item_fn.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                    if let syn::PathArguments::AngleBracketed(angle_args) = &segment.arguments {
                                        if angle_args.args.len() > 1 {
                                            let tuple_type: syn::Type = parse_quote!((#angle_args));
                                            pat_type.ty = Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: path_with_tuple(segment.ident.clone(), tuple_type),
                                            }));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                let mut call_transformer = CallTransformer;
                call_transformer.visit_block_mut(&mut item_fn.block);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator transforms the closure arguments into a single tuple, modifying both the closure's signature and the corresponding function calls. It is designed to test Rust's ability to handle closure argument tuple transformation and uncover potential bugs in trait handling."
    }
}

fn path_with_tuple(ident: Ident, tuple_type: syn::Type) -> syn::Path {
    let mut segments = Punctuated::new();
    segments.push(PathSegment {
        ident,
        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
            colon2_token: None,
            lt_token: Lt::default(),
            args: Punctuated::from_iter(vec![GenericArgument::Type(tuple_type)]),
            gt_token: Gt::default(),
        }),
    });
    syn::Path {
        leading_colon: None,
        segments,
    }
}

struct CallTransformer;

impl VisitMut for CallTransformer {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if node.args.len() > 1 {
            let args: Expr = parse_quote!((#(#node.args),*));
            node.args.clear();
            node.args.push(args);
        }
        visit_mut::VisitMut::visit_expr_call_mut(self, node);
    }
}