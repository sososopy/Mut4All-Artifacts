//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Decltype_Auto_Function_Template_Address_242
 */ 

class MutatorFrontendAction_242 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(242)

private:
    class MutatorASTConsumer_242 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_242(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl*> functionTemplates;
    };
};

//source file
#include "../include/Insert_Decltype_Auto_Function_Template_Address_242.h"

// ========================================================================================================
#define MUT242_OUTPUT 1

void MutatorFrontendAction_242::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check for function template with auto return type
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        // Check if the function's return type is auto
        auto *FD = FTD->getTemplatedDecl();
        if (!FD) return;
        QualType returnType = FD->getReturnType();
        if (!returnType->isAutoType()) return;
        functionTemplates.push_back(FTD);
    }
    // Check for main function
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MainFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody()) return;
        // Insert into main's body
        for (auto *FT : functionTemplates) {
            std::string funcName = FT->getNameAsString();
            std::string varName = "g_" + funcName;
            std::string decl = "decltype(auto) " + varName + " = &" + funcName + "<int>;\n";
            // Insert at the beginning of the function body
            const auto *body = FD->getBody();
            if (body) {
                SourceLocation loc = body->getBeginLoc();
                // Find the start of the compound statement
                if (const auto *compound = dyn_cast<clang::CompoundStmt>(body)) {
                    if (!compound->body_empty()) {
                        SourceLocation insertLoc = compound->getLBracLoc().getLocWithOffset(1);
                        Rewrite.InsertText(insertLoc, decl, true, true);
                    } else {
                        // Insert after the {
                        SourceLocation lBracLoc = compound->getLBracLoc();
                        Rewrite.InsertTextAfter(lBracLoc, decl);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_242::MutatorASTConsumer_242::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST nodes for mutation
    DeclarationMatcher matcher1 = cxxFunctionTemplateDecl(
        has(cxxFunctionDecl(
            returns(qualType(autoType()))
        ))
    ).bind("FunctionTemplate");
    DeclarationMatcher matcher2 = functionDecl(hasName("main")).bind("MainFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}