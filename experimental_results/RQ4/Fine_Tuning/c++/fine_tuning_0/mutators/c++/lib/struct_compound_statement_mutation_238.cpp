//source file
#include "../include/Struct_Compound_Statement_Mutation_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->isStruct() || MT->isUnion()) {
        auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
        llvm::outs() << "/*mut238*/" << definition << '\n';
        definition.insert(definition.find("{") + 1, "{ ");
        definition.insert(definition.rfind("}"), " }");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
      }
    }
}
  
void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}