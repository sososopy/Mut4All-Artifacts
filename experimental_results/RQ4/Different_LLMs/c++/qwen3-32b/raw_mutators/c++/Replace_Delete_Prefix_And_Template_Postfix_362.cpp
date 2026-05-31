//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Delete_Prefix_And_Template_Postfix_362
 */ 
class MutatorFrontendAction_362 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(362)

private:
    class MutatorASTConsumer_362 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_362(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> candidate_classes;
    };
};

//source file
#include "../include/Replace_Delete_Prefix_And_Template_Postfix_362.h"

// ========================================================================================================
#define MUT362_OUTPUT 1

void MutatorFrontendAction_362::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("IncrementClass")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (!CD->isCompleteDefinition())
            return;
        
        // Check if class has both prefix and postfix increment operators
        bool hasPrefix = false, hasPostfix = false;
        for (const auto *M : CD->methods()) {
            if (M->getQualifiedNameAsString() == CD->getNameAsString() + "::operator++") {
                if (M->getNumParams() == 0) hasPrefix = true;
                else if (M->getNumParams() == 1 && M->getParamDecl(0)->getType()->isIntegralType(Result.Context)) hasPostfix = true;
            }
        }
        if (!hasPrefix || !hasPostfix) return;
        
        candidate_classes.push_back(CD);
        
        // Process prefix increment operator
        for (const auto *M : CD->methods()) {
            if (M->getQualifiedNameAsString() == CD->getNameAsString() + "::operator++" && M->getNumParams() == 0) {
                SourceLocation startLoc = M->getSourceRange().getBegin();
                SourceLocation endLoc = M->getSourceRange().getEnd();
                Rewrite.ReplaceText(startLoc, endLoc.getOffset() - startLoc.getOffset(), 
                    M->getNameAsString() + "() = delete");
            }
        }
        
        // Process postfix increment operator
        for (const auto *M : CD->methods()) {
            if (M->getQualifiedNameAsString() == CD->getNameAsString() + "::operator++" && M->getNumParams() == 1) {
                SourceLocation startLoc = M->getSourceRange().getBegin();
                SourceLocation endLoc = M->getSourceRange().getEnd();
                
                std::string templateDecl = "template<class T> ";
                std::string funcBody = " {\n  " + CD->getNameAsString() + " c;\n  ++c; // Triggers deleted prefix increment\n}";
                
                std::string replacement = templateDecl + M->getType().getAsString() + " " + 
                    M->getNameAsString() + "(int)" + funcBody;
                
                Rewrite.ReplaceText(startLoc, endLoc.getOffset() - startLoc.getOffset(), replacement);
            }
        }
    }
}
  
void MutatorFrontendAction_362::MutatorASTConsumer_362::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        hasMethod(cxxMethodDecl(hasName("operator++"), parameterCountIs(0))),
        hasMethod(cxxMethodDecl(hasName("operator++"), parameterCountIs(1)))
    ).bind("IncrementClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}