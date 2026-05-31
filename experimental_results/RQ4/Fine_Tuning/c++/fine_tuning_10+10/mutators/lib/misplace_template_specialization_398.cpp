//source file
#include "../include/Misplace_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        for (auto *Spec : FTD->specializations()) {
            if (Spec->getTemplateSpecializationKind() == TSK_ExplicitSpecialization) {
                specializations.push_back(FTD);
                auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), Spec->getSourceRange());
                Rewrite.RemoveText(Spec->getSourceRange());
                
                std::string structName = dyn_cast<clang::RecordDecl>(FTD->getDeclContext())->getNameAsString();
                std::string templateParams = "template<class T>\n";
                std::string specializationHeader = templateParams + "template<> auto " + structName + "<T>::" + specializationText;
                
                Rewrite.InsertTextAfterToken(FTD->getEndLoc(), "\n/*mut398*/" + specializationHeader);
            }
        }
    }
}

void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasParent(cxxRecordDecl(hasAncestor(namespaceDecl())))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}