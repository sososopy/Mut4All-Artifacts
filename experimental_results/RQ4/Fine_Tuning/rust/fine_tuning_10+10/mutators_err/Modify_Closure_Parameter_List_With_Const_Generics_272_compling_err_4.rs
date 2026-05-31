use syn::{ExprClosure, visit_mut};

pub struct Modify_Closure_Parameter_List_With_Const_Generics_272;

impl Mutator for Modify_Closure_Parameter_List_With_Const_Generics_272 {
    fn name(&self) -> &str {
        "Modify_Closure_Parameter_List_With_Const_Generics_272"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        struct ClosureVisitor;

        impl VisitMut for ClosureVisitor {
            fn visit_expr_closure_mut(&mut self, i: &mut ExprClosure) {
                if i.constness.is_none() {
                    i.constness = Some(syn::token::Const { span: Span::call_site() });
                    i.inputs.insert(0, parse_quote!(const N: i32));
                }
                visit_mut::visit_expr_closure_mut(self, i);
            }
        }

        let mut visitor = ClosureVisitor;
        visitor.visit_file_mut(file);
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures by adding a const generic parameter to their parameter list. This involves identifying closures in the code and modifying their parameter list to include a const generic, such as `const N: i32`. The const generic is not used within the closure body, as the goal is to reproduce scenarios that may lead to compiler instability, as observed in certain bug reports."
    }
}