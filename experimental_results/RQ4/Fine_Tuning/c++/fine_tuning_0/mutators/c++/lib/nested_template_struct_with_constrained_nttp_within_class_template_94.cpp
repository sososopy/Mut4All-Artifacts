//source file
#include "../include/Nested_Template_Struct_with_Constrained_NTTP_within_Class_Template_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_94::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      auto name = CT->getNameAsString();
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CT->getSourceRange());
      content.insert(0, "template <class T> concept MyConcept = true;\n");
      string nested_struct =
          "template <class U, MyConcept auto V> struct MyNestedStruct { "
          "static const int i = 0; };";
      if (content.find('{') != string::npos)
        content.insert(content.find('{') + 1, nested_struct);
      content +=
          "static_assert(" + name + "<void>::MyNestedStruct<int, 0>::i == 0);";
      llvm::outs() << content;
      Rewrite.ReplaceText(CT->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_94::MutatorASTConsumer_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}