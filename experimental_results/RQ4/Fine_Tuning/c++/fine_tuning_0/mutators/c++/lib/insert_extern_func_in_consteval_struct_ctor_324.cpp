//source file
#include "../include/Insert_Extern_Func_In_Consteval_Struct_Ctor_324.h"

// ========================================================================================================
#define MUT324_OUTPUT 1

void MutatorFrontendAction_324::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ExternFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isExternC())
        return;
      extFD = FD;
      llvm::outs() << "extern func: " << extFD->getNameAsString() << '\n';
    } else if (auto *SD =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;
      if (!SD->isStruct())
        return;
      auto ctors = SD->ctors();
      for (auto ctor : ctors) {
        if (ctor->isConsteval()) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ctor->getSourceRange());
          content.insert(content.rfind('}'),
                         extFD->getNameAsString() + "();");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(ctor->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_324::MutatorASTConsumer_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto extern_func_matcher = functionDecl().bind("ExternFunc");
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(extern_func_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}