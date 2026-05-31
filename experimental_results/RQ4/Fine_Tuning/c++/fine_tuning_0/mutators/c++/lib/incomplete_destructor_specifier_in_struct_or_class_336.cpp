//source file
#include "../include/Incomplete_Destructor_Specifier_In_Struct_Or_Class_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto TD = CL->getDestructor();
      if (TD == nullptr)
        return;
      auto def =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      string dtor_definition = "~" + def + "()=default;\n";
      dtor_definition = "/*mut336*/" + dtor_definition;
      if (TD->isImplicit()) {
        llvm::outs() << dtor_definition << '\n';
        Rewrite.ReplaceText(CL->getEndLoc(), 0, dtor_definition);
      }
    }
}

void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}