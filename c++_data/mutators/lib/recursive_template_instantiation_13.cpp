//source file
#include "../include/recursive_template_instantiation_13.h"

// ========================================================================================================
#define MUT13_OUTPUT 1

void MutatorFrontendAction_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->isTemplateInstantiation() || MT->isTemplated()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        auto paramName = MT->getParamDecl(0)->getNameAsString();
        auto returnTypeLoc = declaration.find("->");
        if (returnTypeLoc != std::string::npos) {
          declaration.replace(returnTypeLoc, std::string::npos, "-> decltype((*this)(" + paramName + "))");
        }
        declaration.insert(declaration.find("{") + 1, ";");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_13::MutatorASTConsumer_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation()).bind("TemplateMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}