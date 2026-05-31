//header file
#pragma once
#include "Mutator_base.h"

/**
 * macro_with_struct_member_access_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> macroMap; // Store macro definitions
    };
};

//source file
#include "../include/macro_with_struct_member_access_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const clang::MacroDirective *MD = Result.Nodes.getNodeAs<clang::MacroDirective>("MacroDefs")) {
        if (!Result.SourceManager->isWrittenInMainFile(MD->getLocation()))
            return;
        
        std::string macroName = MD->getMacroInfo()->getDefinitionLoc().printToString(*Result.SourceManager);
        std::string macroBody = MD->getMacroInfo()->getReplacementToken(0).getRawIdentifier().str();
        
        macroMap[macroName] = macroBody;
    }
    
    if (const clang::CallExpr *CE = Result.Nodes.getNodeAs<clang::CallExpr>("MacroCalls")) {
        if (!Result.SourceManager->isWrittenInMainFile(CE->getBeginLoc()))
            return;
        
        const clang::Expr *arg = CE->getArg(1); // Assuming second argument is a struct member
        if (const clang::MemberExpr *ME = llvm::dyn_cast<clang::MemberExpr>(arg)) {
            std::string memberName = ME->getMemberDecl()->getNameAsString();
            std::string macroName = CE->getDirectCallee()->getNameAsString();
            
            if (macroMap.find(macroName) != macroMap.end()) {
                std::string newMacroCall = macroName + "(5, " + memberName + " + 1)"; // Simple mutation
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newMacroCall);
            }
        }
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher macroMatcher = macroDirective().bind("MacroDefs");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasName("compute")))).bind("MacroCalls");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}