//header file
#pragma once
#include "Mutator_base.h"

/**
 * Swap_Pack_Args_412
 */ 
class MutatorFrontendAction_412 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(412)

private:
    class MutatorASTConsumer_412 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_412(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Swap_Pack_Args_412.h"

// ========================================================================================================
#define MUT412_OUTPUT 1

void MutatorFrontendAction_412::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("usingDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        auto sourceRange = UD->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*Result.SourceManager, sourceRange);

        size_t equalsPos = sourceText.find('=');
        if (equalsPos == string::npos)
            return;

        string typePart = sourceText.substr(equalsPos + 1);
        size_t gtPos = typePart.find('>');
        if (gtPos == string::npos)
            return;

        string templateArgs = typePart.substr(0, gtPos);
        size_t ltPos = templateArgs.find('<');
        if (ltPos == string::npos)
            return;

        string templateName = templateArgs.substr(0, ltPos);
        string argsList = templateArgs.substr(ltPos + 1);

        vector<string> args;
        size_t start = 0;
        size_t end = 0;
        while ((end = argsList.find(',', start)) != string::npos) {
            string arg = argsList.substr(start, end - start);
            args.push_back(arg);
            start = end + 1;
        }
        string lastArg = argsList.substr(start);
        args.push_back(lastArg);

        if (!args.empty()) {
            string last = args.back();
            if (last.find("...") == string::npos) {
                bool hasPack = false;
                for (size_t i = 0; i < args.size() - 1; ++i) {
                    if (args[i].find("...") != string::npos) {
                        hasPack = true;
                        break;
                    }
                }
                if (hasPack) {
                    string newArgs = last + ", " + argsList.substr(0, argsList.length() - last.length() - 1);
                    string newTypePart = templateName + "<" + newArgs + ">";
                    string newSourceText = sourceText.substr(0, equalsPos + 1) + newTypePart + sourceText.substr(equalsPos + 1 + typePart.length());
                    Rewrite.ReplaceText(sourceRange, newSourceText);
                }
            }
        }
    }
}

void MutatorFrontendAction_412::MutatorASTConsumer_412::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingDecl(hasType(aliasTemplateSpecializationType())).bind("usingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}