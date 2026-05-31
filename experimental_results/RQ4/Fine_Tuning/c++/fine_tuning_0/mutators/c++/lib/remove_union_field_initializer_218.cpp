//source file
#include "../include/Remove_Union_Field_Initializer_218.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionFieldWithInit")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasInClassInitializer())
        return;
      auto field_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << field_decl << '\n';
      auto equal_pos = field_decl.find('=');
      if (equal_pos == string::npos)
        return;
      field_decl.erase(equal_pos + 1);
      llvm::outs() << field_decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), field_decl);
    }
}
  
void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = fieldDecl(hasParent(cxxRecordDecl(isUnion())),
                             hasInClassInitializer(expr()))
                       .bind("UnionFieldWithInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}