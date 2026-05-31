//source file
#include "../include/constexpr_destructor_modification_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithDestructor")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;

        if (!CL->isCompleteDefinition())
            return;

        auto *Destructor = CL->getDestructor();
        if (!Destructor)
            return;

        if (Destructor->isImplicit()) {
            std::string className = CL->getNameAsString();
            std::string dtorText = "constexpr ~" + className + "() = default;\n";
            Rewrite.InsertTextAfter(CL->getEndLoc().getLocWithOffset(-1), "/*mut37*/" + dtorText);
        } else {
            if (Destructor->hasBody()) {
                std::string dtorText = Rewrite.getRewrittenText(Destructor->getSourceRange());
                if (dtorText.find("constexpr") == std::string::npos) {
                    size_t pos = dtorText.find("~");
                    if (pos != std::string::npos) {
                        dtorText.insert(pos, "constexpr ");
                        Rewrite.ReplaceText(Destructor->getSourceRange(), dtorText);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxDestructorDecl())).bind("ClassWithDestructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}