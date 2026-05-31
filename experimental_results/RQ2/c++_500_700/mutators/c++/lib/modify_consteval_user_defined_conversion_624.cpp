//source file
#include "../include/modify_consteval_user_defined_conversion_624.h"

// ========================================================================================================
#define MUT624_OUTPUT 1

void MutatorFrontendAction_624::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isConsteval()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("consteval");
        if (pos != std::string::npos) {
            declaration.erase(pos, std::string("consteval").length());
            size_t bodyPos = declaration.find("{");
            if (bodyPos != std::string::npos) {
                declaration.replace(bodyPos, declaration.length() - bodyPos, "{ return 3.14; }");
            }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_624::MutatorASTConsumer_624::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl(isConsteval()).bind("ConversionFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}