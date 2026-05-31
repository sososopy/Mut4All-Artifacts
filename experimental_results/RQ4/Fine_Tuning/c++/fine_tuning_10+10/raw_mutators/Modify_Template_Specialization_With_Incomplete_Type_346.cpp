//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Specialization_With_Incomplete_Type_346
 */ 
class MutatorFrontendAction_346 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(346)

private:
    class MutatorASTConsumer_346 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_346(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_template_specialization_with_incomplete_type_346.h"

// ========================================================================================================
#define MUT346_OUTPUT 1

void MutatorFrontendAction_346::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        const TemplateArgumentList &args = TS->getTemplateArgs();
        std::string newSpecialization = "Mask<IncompleteType, 16>";

        if (args.size() == 4) {
            const TemplateArgument &lastArg = args[3];
            if (lastArg.getKind() == TemplateArgument::Type) {
                QualType qt = lastArg.getAsType();
                if (qt->getAsRecordDecl() && qt->getAsRecordDecl()->getName() == "Mask") {
                    SourceRange range = TS->getSourceRange();
                    std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
                    size_t pos = originalText.rfind("Mask<");
                    if (pos != std::string::npos) {
                        originalText.replace(pos, std::string::npos, newSpecialization);
                        Rewrite.ReplaceText(range, originalText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_346::MutatorASTConsumer_346::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasName("StaticArrayImpl")).bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}