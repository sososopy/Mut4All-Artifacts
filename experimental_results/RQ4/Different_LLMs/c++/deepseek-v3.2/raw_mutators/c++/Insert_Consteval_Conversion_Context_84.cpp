//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Consteval_Conversion_Context_84
 */ 
class MutatorFrontendAction_84 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(84)

private:
    class MutatorASTConsumer_84 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_84(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> constevalTypes;
        std::vector<const clang::Type *> availableTypes;
    };
};

//source file
#include "../include/Mutator_Insert_Consteval_Conversion_Context_84.h"

// ========================================================================================================
#define MUT84_OUTPUT 1

void MutatorFrontendAction_84::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ConstevalType")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition())
            return;
        
        // Check if the record has a consteval constructor or conversion operator
        bool hasConstevalMember = false;
        for (auto *D : RD->decls()) {
            if (auto *Ctor = llvm::dyn_cast<clang::CXXConstructorDecl>(D)) {
                if (Ctor->isConsteval()) {
                    hasConstevalMember = true;
                    break;
                }
            } else if (auto *Conv = llvm::dyn_cast<clang::CXXConversionDecl>(D)) {
                if (Conv->isConsteval()) {
                    hasConstevalMember = true;
                    break;
                }
            }
        }
        
        if (hasConstevalMember) {
            constevalTypes.push_back(RD);
        }
    }
    //Check whether the matched AST node is the target node
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionBody")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        
        // Collect available fundamental types from the function's context
        auto &Context = *Result.Context;
        availableTypes.push_back(Context.IntTy.getTypePtr());
        availableTypes.push_back(Context.DoubleTy.getTypePtr());
        availableTypes.push_back(Context.FloatTy.getTypePtr());
        availableTypes.push_back(Context.CharTy.getTypePtr());
        availableTypes.push_back(Context.BoolTy.getTypePtr());
        
        // If we have consteval types and available fundamental types, create mutation
        if (!constevalTypes.empty() && !availableTypes.empty()) {
            // Get a random consteval type
            size_t typeIdx = getrandom::getRandomIndex(constevalTypes.size() - 1);
            auto *constevalType = constevalTypes[typeIdx];
            
            // Get a random fundamental type
            size_t fundIdx = getrandom::getRandomIndex(availableTypes.size() - 1);
            auto *fundType = availableTypes[fundIdx];
            
            // Create the mutation statement
            std::string typeName = fundType->getCanonicalTypeInternal().getAsString();
            std::string constevalName = constevalType->getNameAsString();
            
            // Choose between different initialization patterns
            int pattern = getrandom::getRandomIndex(2);
            std::string mutation;
            
            switch (pattern) {
                case 0:
                    // Functional cast with list initialization
                    mutation = typeName + " var_84 = " + constevalName + "{};";
                    break;
                case 1:
                    // Direct list initialization with conversion
                    mutation = typeName + " var_84{" + constevalName + "{}};";
                    break;
                case 2:
                    // Assignment with list initialization
                    mutation = typeName + " var_84;\nvar_84 = " + constevalName + "{};";
                    break;
            }
            
            mutation = "\n/*mut84*/" + mutation + "\n";
            
            // Insert the mutation at the beginning of the function body
            if (auto *Body = FD->getBody()) {
                SourceLocation InsertLoc = Body->getBeginLoc().getLocWithOffset(1);
                Rewrite.InsertText(InsertLoc, mutation, true, true);
            }
        }
    }
}
  
void MutatorFrontendAction_84::MutatorASTConsumer_84::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // Match consteval types (structs/classes with consteval constructors or conversion operators)
    DeclarationMatcher constevalMatcher = cxxRecordDecl(
        has(cxxMethodDecl(anyOf(
            cxxConstructorDecl(isConsteval()),
            cxxConversionDecl(isConsteval())
        ))
    )).bind("ConstevalType");
    
    // Match function bodies where we can insert the mutation
    DeclarationMatcher functionMatcher = functionDecl(
        hasBody(compoundStmt()),
        unless(cxxConstructorDecl()),
        unless(cxxDestructorDecl())
    ).bind("FunctionBody");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constevalMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}