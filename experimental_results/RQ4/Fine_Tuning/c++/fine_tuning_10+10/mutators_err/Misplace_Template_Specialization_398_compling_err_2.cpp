//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misplace_Template_Specialization_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionTemplateDecl*> specializations;
    };
};

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
    DeclarationMatcher matcher = functionTemplateDecl(hasParent(cxxRecordDecl(has(cxxTemplateDecl())))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}