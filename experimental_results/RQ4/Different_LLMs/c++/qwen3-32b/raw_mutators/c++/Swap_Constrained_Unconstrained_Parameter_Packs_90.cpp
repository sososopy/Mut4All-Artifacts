//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Constrained_Unconstrained_Parameter_Packs_90
 */ 
class MutatorFrontendAction_90 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(90)

private:
    class MutatorASTConsumer_90 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_90(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Swap_Constrained_Unconstrained_Parameter_Packs_90.h"

// ========================================================================================================
#define MUT90_OUTPUT 1

void MutatorFrontendAction_90::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        //Filter nodes in header files
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                           funcDecl->getLocation()))
            return;

        SmallVector<clang::ParmVarDecl*, 4> unconstrainedPacks;
        SmallVector<clang::ParmVarDecl*, 4> constrainedPacks;

        for (auto *param : funcDecl->parameters()) {
            QualType paramType = param->getType();
            if (auto *packType = paramType->getAs<clang::PackExpansionType>()) {
                if (auto *conceptExpr = dyn_cast<clang::ConceptSpecializationExpr>(packType->getPattern())) {
                    constrainedPacks.push_back(param);
                } else {
                    unconstrainedPacks.push_back(param);
                }
            }
        }

        if (unconstrainedPacks.empty() || constrainedPacks.empty()) {
            return;
        }

        std::string newParamList;
        for (size_t i = 0; i < unconstrainedPacks.size(); ++i) {
            newParamList += stringutils::getSourceText(Rewrite.getSourceMgr(), unconstrainedPacks[i]->getSourceRange());
            if (i != unconstrainedPacks.size() - 1 || !constrainedPacks.empty()) {
                newParamList += ", ";
            }
        }
        for (size_t i = 0; i < constrainedPacks.size(); ++i) {
            newParamList += stringutils::getSourceText(Rewrite.getSourceMgr(), constrainedPacks[i]->getSourceRange());
            if (i != constrainedPacks.size() - 1) {
                newParamList += ", ";
            }
        }

        clang::SourceLocation lParenLoc = funcDecl->getLParenLoc();
        clang::SourceLocation rParenLoc = funcDecl->getRParenLoc();

        if (lParenLoc.isInvalid() || rParenLoc.isInvalid()) {
            return;
        }

        Rewrite.ReplaceText(clang::SourceRange(lParenLoc, rParenLoc), newParamList);
    }
}
  
void MutatorFrontendAction_90::MutatorASTConsumer_90::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
        hasAnyParameter(
            parameterDecl(
                hasType(
                    packExpansionType(
                        hasType(
                            conceptSpecializationExpr()
                        )
                    )
                )
            )
        ),
        hasAnyParameter(
            parameterDecl(
                hasType(
                    packExpansionType(
                        unless(
                            hasType(
                                conceptSpecializationExpr()
                            )
                        )
                    )
                )
            )
        )
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}