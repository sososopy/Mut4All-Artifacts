//source file
#include "../include/Annotate_With_Function_Or_Method_Declaration_403.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        if (MT->isVirtual()) {
          llvm::outs() << "virtual\n";
        }
        auto parent = MT->getParent();
        if (parent->isLambda())
          return;
        auto name = parent->getNameAsString();
        if (name == "")
          return;
        auto template_args = parent->getDescribedClassTemplate();
        if (template_args == nullptr)
          return;
        auto args = stringutils::rangetoStr(*(Result.SourceManager),
                                            template_args->getSourceRange());
        llvm::outs() << args << '\n';
        declaration.insert(declaration.find(" "),
                           "[[clang::annotate(\"test\", (9, (void)" + args +
                               "{}))]] ");
        int choice = getrandom::getRandomIndex(1);
        if (choice) {
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}