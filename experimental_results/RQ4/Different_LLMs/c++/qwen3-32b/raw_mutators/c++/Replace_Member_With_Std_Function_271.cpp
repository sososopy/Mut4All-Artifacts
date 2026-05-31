//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_With_Std_Function_271
 */ 
class MutatorFrontendAction_271 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(271)

private:
    class MutatorASTConsumer_271 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_271(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> target_classes;
    };
};

//source file
#include "../include/Replace_Member_With_Std_Function_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        // Collect candidate methods to replace
        std::vector<const clang::CXXMethodDecl*> methods;
        for (const auto *method : CD->methods()) {
            if (!method->isConstructor() && method->isThisDeclarationADefinition()) {
                methods.push_back(method);
            }
        }
        
        if (methods.empty()) return;
        
        // Select first non-constructor method to replace
        const auto *target_method = methods[0];
        const auto *constructor = CD->getConstructor();
        if (!constructor) return;
        
        // Replace method with standard function name member
        std::string std_func_name = "memmove";
        std::string new_member = "int " + std_func_name + ";";
        SourceLocation method_loc = target_method->getBeginLoc();
        Rewrite.ReplaceText(method_loc, target_method->getSourceRange(), new_member);
        
        // Add using directive
        SourceLocation insert_loc = CD->getEndLoc();
        Rewrite.InsertText(insert_loc, "\n    using ::" + std_func_name + ";", true, true);
        
        // Modify constructor initializer list
        if (auto *ctor = CD->getConstructor()) {
            if (auto *ctor_body = ctor->getBody()) {
                std::string body_text = stringutils::rangetoStr(*Result.SourceManager, ctor_body->getSourceRange());
                size_t colon_pos = body_text.find(':');
                
                if (colon_pos != std::string::npos) {
                    std::string new_initializer = std_func_name + "()";
                    body_text.insert(colon_pos + 1, new_initializer);
                    Rewrite.ReplaceText(ctor_body->getSourceRange(), body_text);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(cxxConstructorDecl()),
        hasDescendant(cxxMethodDecl())
    ).bind("classDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}