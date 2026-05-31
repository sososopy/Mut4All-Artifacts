//source file
#include "../include/Replace_Member_Function_Call_With_Dependent_Base_Call_170.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_170::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;
      //Get the source code text of target node
      auto callee = MT->getMethodDecl();
      if (callee == nullptr)
        return;
      auto DL = callee->getParent();
      if (DL == nullptr)
        return;
      if (!DL->isDependentContext())
        return;
      auto bases = DL->bases();
      if (bases.empty())
        return;
      auto base = bases.begin();
      auto base_type = base->getType();
      auto base_decl = base_type->getAsCXXRecordDecl();
      if (base_decl == nullptr)
        return;
      auto base_methods = base_decl->methods();
      if (base_methods.empty())
        return;
      auto base_method = base_methods.begin();
      auto base_method_name = base_method->getNameAsString();
      auto base_method_return_type =
          base_method->getReturnType().getAsString();
      auto base_method_decl = base_method->getParent();
      auto base_method_decl_name = base_method_decl->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto replacement = "/*mut170*/this->*static_cast<" +
                         base_method_return_type + " (" +
                         base_method_decl_name + "::*)()const>(&" +
                         base_method_decl_name + "::" + base_method_name +
                         ")()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_170::MutatorASTConsumer_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxMemberCallExpr().bind("MemberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}