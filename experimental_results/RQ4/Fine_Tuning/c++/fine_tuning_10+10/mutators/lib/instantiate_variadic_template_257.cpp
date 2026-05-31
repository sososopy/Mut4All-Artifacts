//source file
#include "../include/instantiate_variadic_template_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("VariadicTemplate")) {
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(VT->getLocation()))
        return;

      std::string newTypeDecl = "struct NewStruct {};\n";
      std::string instantiationEmpty = VT->getNameAsString() + "<> instanceEmpty;\n";
      std::string instantiationRepeated = VT->getNameAsString() + "<int, int> instanceRepeated;\n";
      std::string instantiationNewType = VT->getNameAsString() + "<NewStruct, NewStruct> instanceNewType;\n";

      SourceLocation insertLoc = VT->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertTextAfter(insertLoc, "/*mut257*/\n" + newTypeDecl + instantiationEmpty + instantiationRepeated + instantiationNewType);
    }
}

void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateTypeParmDecl(unless(isImplicit())))).bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}