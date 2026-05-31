//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Arg_With_Unexpanded_Pack_215
 */ 
class MutatorFrontendAction_215 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(215)

private:
    class MutatorASTConsumer_215 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_215(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Arg_With_Unexpanded_Pack_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        // Find a parameter pack in the current scope
        clang::DeclContext *DC = CE->getDeclContext();
        const clang::TemplateTypeParmDecl *PackDecl = nullptr;
        while (DC) {
            if (auto *TD = dyn_cast<clang::FunctionTemplateDecl>(DC)) {
                for (const auto *Param : TD->getTemplateParameters()) {
                    if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                        if (TTP->isParameterPack()) {
                            PackDecl = TTP;
                            break;
                        }
                    }
                }
                if (PackDecl)
                    break;
            } else if (auto *CD = dyn_cast<clang::ClassTemplateDecl>(DC)) {
                for (const auto *Param : CD->getTemplateParameters()) {
                    if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                        if (TTP->isParameterPack()) {
                            PackDecl = TTP;
                            break;
                        }
                    }
                }
                if (PackDecl)
                    break;
            }
            DC = DC->getParent();
        }

        if (!PackDecl)
            return;

        // Now, replace one of the arguments with the parameter pack
        // Select a random argument to replace
        unsigned NumArgs = CE->getNumArgs();
        if (NumArgs == 0)
            return;

        unsigned ArgIdx = getrandom::getRandomIndex(NumArgs); // Assuming getrandom::getRandomIndex is a helper function

        // Get the source range of the selected argument
        clang::SourceLocation ArgLocStart = CE->getArg(ArgIdx)->getBeginLoc();
        clang::SourceLocation ArgLocEnd = CE->getArg(ArgIdx)->getEndLoc();

        // Get the name of the parameter pack
        std::string PackName = PackDecl->getNameAsString();

        // Replace the argument with the pack name
        Rewrite.ReplaceText(SourceRange(ArgLocStart, ArgLocEnd), PackName);
    }
}

void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = callExpr().bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}