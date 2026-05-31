//source file
#include "../include/modify_template_conversion_operator_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *MT = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->getDescribedFunctionTemplate()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string newTargetType = MT->getDescribedFunctionTemplate()->getTemplateParameters()->getParam(0)->getNameAsString();
        std::string originalTargetType = MT->getReturnType().getAsString();
        size_t pos = declaration.find(originalTargetType);
        if (pos != std::string::npos) {
          declaration.replace(pos, originalTargetType.length(), newTargetType);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl(isTemplateInstantiation()).bind("ConversionOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}