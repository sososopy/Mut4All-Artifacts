//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_instantiation_with_incomplete_array_530
 */ 
class MutatorFrontendAction_530 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(530)

private:
    class MutatorASTConsumer_530 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_530(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_instantiation_with_incomplete_array_530.h"

// ========================================================================================================
#define MUT530_OUTPUT 1

void MutatorFrontendAction_530::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Call = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateFunc")) {
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getLocation()))
            return;

        for (auto *Spec : Call->specializations()) {
            if (Spec->getTemplateSpecializationKind() == TSK_ExplicitSpecialization) {
                const TemplateArgumentList *Args = Spec->getTemplateArgsAsWritten();
                if (Args) {
                    for (unsigned i = 0; i < Args->size(); ++i) {
                        const TemplateArgument &Arg = Args->get(i);
                        if (Arg.getKind() == TemplateArgument::Expression) {
                            if (auto *InitList = dyn_cast<InitListExpr>(Arg.getAsExpr())) {
                                if (InitList->getNumInits() > 1) {
                                    std::string newInit = "{";
                                    for (unsigned j = 0; j < InitList->getNumInits() - 1; ++j) {
                                        newInit += Lexer::getSourceText(CharSourceRange::getTokenRange(InitList->getInit(j)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
                                        if (j < InitList->getNumInits() - 2) {
                                            newInit += ", ";
                                        }
                                    }
                                    newInit += "}";
                                    Rewrite.ReplaceText(InitList->getSourceRange(), newInit);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_530::MutatorASTConsumer_530::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(initListExpr())).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}