//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Consteval_Constructor_With_Constexpr_Call_96
 */ 
class MutatorFrontendAction_96 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(96)
private:
    class MutatorASTConsumer_96 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_96(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> constexpr_funcs;
        std::set<std::string> structs_with_extern;
    };
};

//source file
#include "../include/Introduce_Consteval_Constructor_With_Constexpr_Call_96.h"
#include <regex>
#include <string>

// ========================================================================================================
#define MUT96_OUTPUT 1

void MutatorFrontendAction_96::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("constexpr_func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isConstexpr() && !FD->isConstexprConstructor()) {
            constexpr_funcs.push_back(FD->getNameAsString());
        }
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (!CD->hasBody()) {
            return;
        }
        auto *parentStruct = CD->getParent();
        if (!parentStruct || !parentStruct->isStruct() || !parentStruct->isCompleteDefinition())
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(parentStruct->getLocation()))
            return;
        std::string structName = parentStruct->getNameAsString();
        if (constexpr_funcs.empty() && structs_with_extern.find(structName) == structs_with_extern.end()) {
            auto structLoc = parentStruct->getBeginLoc();
            std::string externDecl = "extern constexpr void func();\n";
            Rewrite.InsertTextBefore(structLoc, externDecl);
            structs_with_extern.insert(structName);
        }
        auto constructorRange = CD->getSourceRange();
        auto constructorText = stringutils::rangetoStr(*Result.SourceManager, constructorRange);
        std::string newConstructorText = constructorText;
        if (newConstructorText.find("constexpr") != std::string::npos) {
            newConstructorText = std::regex_replace(newConstructorText, std::regex("\\bconstexpr\\b"), "consteval");
        } else if (newConstructorText.find("default") != std::string::npos) {
            newConstructorText = std::regex_replace(newConstructorText, std::regex("\\bdefault\\b"), "consteval");
        } else {
            std::string constructorName = CD->getNameAsString();
            size_t namePos = newConstructorText.find(constructorName);
            if (namePos != std::string::npos) {
                newConstructorText.insert(namePos, "consteval ");
            } else {
                return;
            }
        }
        std::string funcCall;
        if (constexpr_funcs.empty()) {
            funcCall = "func();";
        } else {
            int index = getrandom::getRandomIndex(constexpr_funcs.size() - 1);
            funcCall = constexpr_funcs[index] + "();";
        }
        size_t bodyStart = newConstructorText.find("{");
        if (bodyStart != std::string::npos) {
            newConstructorText.insert(bodyStart + 1, "\n    " + funcCall + "\n");
        } else {
            return;
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constructorRange), newConstructorText);
    }
}
  
void MutatorFrontendAction_96::MutatorASTConsumer_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = functionDecl(isConstexpr(), unless(isConstexprConstructor())).bind("constexpr_func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.matchAST(Context);

    MatchFinder constructorFinder;
    DeclarationMatcher constructorMatcher = cxxConstructorDecl(unless(isConsteval())).bind("constructor");
    constructorFinder.addMatcher(constructorMatcher, &callback);
    constructorFinder.matchAST(Context);
}