//source file
#include "../include/Apply_Attribute_To_Template_Parameter_Pack_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunctions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Insert a lambda function with a Clang-specific attribute applied to the parameter pack
      declaration.insert(declaration.rfind("}"), "[] () __attribute__((diagnose_if(Args, \"message\", \"error\"))) {}();");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl()), has(cxxMethodDecl())).bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}