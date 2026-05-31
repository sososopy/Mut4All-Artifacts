//header file
#pragma once
#include "Mutator_base.h"

/**
 * PartialSpecializationBaseParameterMismatch_376
 */ 
class MutatorFrontendAction_376 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(376)

private:
    class MutatorASTConsumer_376 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_376(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/PartialSpecializationBaseParameterMismatch_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PartialSpec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("partialSpec")) {
        if (!PartialSpec || !Result.Context->getSourceManager().isWrittenInMainFile(PartialSpec->getLocation()))
            return;

        for (const auto &Base : PartialSpec->bases()) {
            QualType BaseType = Base.getType();
            if (auto *TST = BaseType->getAs<clang::TemplateSpecializationType>()) {
                clang::TemplateArgumentListInfo *Args = TST->getArgs();
                if (!Args) continue;

                for (unsigned i = 0; i < Args->size(); ++i) {
                    clang::TemplateArgument Arg = Args->getArgument(i);
                    if (Arg.getKind() == clang::TemplateArgument::Type) {
                        clang::Type *T = Arg.getAsType().getTypePtr();
                        if (auto *TTP = clang::dyn_cast<clang::TemplateTypeParmType>(T)) {
                            clang::TemplateParameterList *TPL = PartialSpec->getTemplateParameters();
                            std::vector<clang::TemplateTypeParmDecl *> PartialParams;
                            for (unsigned j = 0; j < TPL->size(); ++j) {
                                clang::TemplateParameter *Param = TPL->getParam(j);
                                if (auto *TTPD = clang::dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                                    PartialParams.push_back(TTPD);
                                }
                            }

                            clang::TemplateTypeParmDecl *ReplacementParam = nullptr;
                            for (auto *P : PartialParams) {
                                if (P != TTP) {
                                    ReplacementParam = P;
                                    break;
                                }
                            }

                            if (ReplacementParam) {
                                clang::TemplateArgumentLoc ArgLoc = Args->getArgumentLoc(i);
                                clang::SourceLocation Start = ArgLoc.getSourceRange().getBegin();
                                clang::SourceLocation End = ArgLoc.getSourceRange().getEnd();
                                std::string ReplacementText = ReplacementParam->getNameAsString();
                                Rewrite.ReplaceText(clang::SourceRange(Start, End), ReplacementText);
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("partialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}