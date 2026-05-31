//source file
#include "../include/Replace_Base_Class_Function_Call_With_Static_Cast_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->hasBody()) {
        auto body = MT->getBody();
        for (auto it = body->child_begin(); it != body->child_end(); ++it) {
          if (auto call = dyn_cast<CXXMemberCallExpr>(*it)) {
            auto callee = call->getMethodDecl();
            if (callee->isVirtual() == false) {
              auto parent = callee->getParent();
              if (MT->getParent()->isDerivedFrom(parent)) {
                auto callee_name = callee->getNameAsString();
                auto callee_return_type =
                    callee->getReturnType().getAsString();
                string callee_arg_type = "";
                for (auto param = callee->param_begin();
                     param != callee->param_end(); ++param) {
                  callee_arg_type +=
                      (*param)->getType().getCanonicalType().getAsString();
                  if (param + 1 != callee->param_end())
                    callee_arg_type += ",";
                }
                auto call_expr =
                    stringutils::rangetoStr(*(Result.SourceManager),
                                            call->getSourceRange());
                //Perform mutation on the source code text by applying string replacement
                auto static_cast_expr = "(this->*static_cast<" +
                                        callee_return_type + " (" +
                                        parent->getNameAsString() + "::*)(" +
                                        callee_arg_type + ")>(&" +
                                        parent->getNameAsString() + "::" +
                                        callee_name + "))()";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(
                    CharSourceRange::getTokenRange(call->getSourceRange()),
                    static_cast_expr);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}