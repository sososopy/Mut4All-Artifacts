//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variadic_Template_Friend_Function_125
 */ 
class MutatorFrontendAction_125 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(125)

private:
    class MutatorASTConsumer_125 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_125(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *FriendFunctionDecl = nullptr;
    };
};

//source file
#include "../include/modify_variadic_template_friend_function_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("VariadicTemplate")) {
        //Filter nodes in header files
        if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(VT->getLocation()))
            return;
        
        for (auto *Spec : VT->specializations()) {
            for (auto *Decl : Spec->decls()) {
                if (auto *FD = llvm::dyn_cast<clang::FunctionDecl>(Decl)) {
                    if (FD->isFriend() && FD->getTemplateSpecializationArgs()) {
                        FriendFunctionDecl = FD;
                        break;
                    }
                }
            }
        }

        if (FriendFunctionDecl) {
            //Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FriendFunctionDecl->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string modifiedDeclaration = declaration;
            size_t pos = modifiedDeclaration.find("...");
            if (pos != std::string::npos) {
                modifiedDeclaration.insert(pos, ", Ts");
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FriendFunctionDecl->getSourceRange()), modifiedDeclaration);
        }
    }
}

void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(has(friendDecl())))).bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}