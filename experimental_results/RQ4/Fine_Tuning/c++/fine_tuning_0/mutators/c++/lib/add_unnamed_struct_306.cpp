//source file
#include "../include/Add_Unnamed_Struct_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if ((!SD->isStruct() && !SD->isClass() && !SD->isUnion()) || SD->isLambda())
        return;
      if (!SD->isCompleteDefinition())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());
      auto unnamed_struct = "\n\t/*mut306*/struct { int mut_306; }; \n";
      if (content.find('{') != string::npos)
        content.insert(content.find('{') + 1, unnamed_struct);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}