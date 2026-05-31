//source file
#include "../include/Mutate_Struct_Member_Function_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isStruct())
        return;
      auto methods = MT->methods();
      for (auto method : methods) {
        auto method_name = method->getNameAsString();
        llvm::outs() << method_name << '\n';
        auto method_decl =
            stringutils::rangetoStr(*(Result.SourceManager), method->getSourceRange());
        llvm::outs() << method_decl << '\n';
        auto pos = method_decl.find(method_name);
        method_decl = "static " + method_decl.substr(pos);
        llvm::outs() << method_decl << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), method_decl);
      }
    }
}
  
void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}