//source file
#include "../include/Target_Struct_With_Forward_Declaration_And_Template_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStruct() == false)
        return;
      if (DL->isCompleteDefinition() == true)
        return;
      if (DL->isTemplated() == false)
        return;
      auto DLname = DL->getNameAsString();
      auto DLtemplate = DL->getDescribedTemplate();
      if (DLtemplate == nullptr)
        return;
      auto DLtemplateparam = DLtemplate->getTemplateParameters();
      if (DLtemplateparam->size() == 0)
        return;
      auto DLtemplateparam0 = DLtemplateparam->getParam(0);
      auto DLtemplateparam0name = DLtemplateparam0->getNameAsString();
      llvm::outs() << DLname << '\n';
      auto definition =
          DLname + " { void func();};\n template<class " + DLtemplateparam0name +
          "> void " + DLname + "<" + DLtemplateparam0name + ">::func() {" +
          DLname + "<" + DLtemplateparam0name + ">* ptr; }";
      definition = "/*mut228*/" + definition;
      llvm::outs() << definition << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, definition);
    }
}
  
void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}