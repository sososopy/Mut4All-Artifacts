//source file
#include "../include/Specialize_Member_Function_Template_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      auto fields = MT->decls();
      for (auto field : fields) {
        if (field->getKind() == Decl::Kind::FunctionTemplate) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
          llvm::outs() << content;
          auto name = llvm::dyn_cast<NamedDecl>(field)->getNameAsString();
          auto spec = "template<> auto " + name +
                      "<1>() { return [](auto x) { return x; }; } ";
          content.insert(content.rfind('}'), spec);
          llvm::outs() << content;
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}