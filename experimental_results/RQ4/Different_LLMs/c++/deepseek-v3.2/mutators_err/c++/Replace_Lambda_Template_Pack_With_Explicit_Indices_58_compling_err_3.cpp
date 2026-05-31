//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Template_Pack_With_Explicit_Indices_58
 */ 
class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)

private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> targetLambdas;
        std::vector<const clang::CallExpr *> indexSequenceCalls;
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_Template_Pack_With_Explicit_Indices_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;
        
        auto *Method = LE->getCallOperator();
        if (!Method || !Method->isTemplateInstantiation())
            return;
        
        auto *TemplateParams = Method->getTemplateSpecializationInfo();
        if (!TemplateParams)
            return;
        
        const auto *Args = TemplateParams->TemplateArguments;
        if (!Args || Args->size() != 1)
            return;
        
        if (Args->get(0).getKind() != clang::TemplateArgument::Pack)
            return;
        
        targetLambdas.push_back(LE);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("indexSequenceCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;
        
        auto *Callee = CE->getDirectCallee();
        if (!Callee)
            return;
        
        std::string CalleeName = Callee->getNameAsString();
        if (CalleeName != "make_index_sequence" && CalleeName != "make_integer_sequence")
            return;
        
        indexSequenceCalls.push_back(CE);
    }
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("finalLambda")) {
        if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(LE->getBeginLoc()))
            return;
        
        if (targetLambdas.empty() || indexSequenceCalls.empty())
            return;
        
        size_t lambdaIndex = getrandom::getRandomIndex(targetLambdas.size() - 1);
        size_t callIndex = getrandom::getRandomIndex(indexSequenceCalls.size() - 1);
        
        auto *TargetLambda = targetLambdas[lambdaIndex];
        auto *IndexCall = indexSequenceCalls[callIndex];
        
        auto *Method = TargetLambda->getCallOperator();
        auto *TemplateParams = Method->getTemplateSpecializationInfo();
        const auto *Args = TemplateParams->TemplateArguments;
        const auto &Pack = Args->get(0).getPackAsArray();
        
        if (Pack.empty())
            return;
        
        auto *IndexArg = IndexCall->getArg(0);
        if (!IndexArg)
            return;
        
        clang::Expr::EvalResult Value;
        if (!IndexArg->EvaluateAsInt(Value, *Result.Context))
            return;
        
        int N = Value.Val.getInt().getExtValue();
        if (N <= 0 || N > 20)
            return;
        
        std::string PackName = "Is";
        if (Pack[0].getKind() == clang::TemplateArgument::Type) {
            auto *Type = Pack[0].getAsType().getTypePtr();
            if (Type->isTemplateTypeParmType())
                PackName = Type->getAsTemplateTypeParmType()->getIdentifier()->getName().str();
        }
        
        std::string NewTemplateParams;
        std::string NewIndexSequence;
        std::string NewExpansion;
        
        for (int i = 0; i < N; ++i) {
            if (i > 0) {
                NewTemplateParams += ", ";
                NewIndexSequence += ", ";
                NewExpansion += ", ";
            }
            NewTemplateParams += "std::size_t " + PackName + std::to_string(i);
            NewIndexSequence += PackName + std::to_string(i);
            NewExpansion += PackName + std::to_string(i);
        }
        
        auto LambdaRange = TargetLambda->getSourceRange();
        std::string LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
        
        size_t TemplateStart = LambdaText.find("<");
        size_t TemplateEnd = LambdaText.find(">", TemplateStart);
        if (TemplateStart == std::string::npos || TemplateEnd == std::string::npos)
            return;
        
        std::string OldTemplate = LambdaText.substr(TemplateStart, TemplateEnd - TemplateStart + 1);
        std::string NewTemplate = "<" + NewTemplateParams + ">";
        
        LambdaText.replace(TemplateStart, TemplateEnd - TemplateStart + 1, NewTemplate);
        
        size_t ParenStart = LambdaText.find("(", TemplateStart);
        size_t ParenEnd = LambdaText.find(")", ParenStart);
        if (ParenStart == std::string::npos || ParenEnd == std::string::npos)
            return;
        
        std::string OldParams = LambdaText.substr(ParenStart, ParenEnd - ParenStart + 1);
        std::string NewParams = "(std::index_sequence<" + NewIndexSequence + ">)";
        
        LambdaText.replace(ParenStart, ParenEnd - ParenStart + 1, NewParams);
        
        size_t BodyStart = LambdaText.find("{", ParenEnd);
        if (BodyStart == std::string::npos)
            return;
        
        std::string BodyText = LambdaText.substr(BodyStart);
        size_t ExpansionPos = BodyText.find("Is...");
        while (ExpansionPos != std::string::npos) {
            BodyText.replace(ExpansionPos, 4, NewExpansion);
            ExpansionPos = BodyText.find("Is...", ExpansionPos + NewExpansion.size());
        }
        
        LambdaText = LambdaText.substr(0, BodyStart) + BodyText;
        
        LambdaText = "/*mut58*/" + LambdaText;
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), LambdaText);
        
        targetLambdas.clear();
        indexSequenceCalls.clear();
    }
}
  
void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    
    StatementMatcher lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasAnyName("make_index_sequence", "make_integer_sequence")))).bind("indexSequenceCall");
    StatementMatcher finalMatcher = lambdaExpr().bind("finalLambda");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(finalMatcher, &callback);
    matchFinder.matchAST(Context);
}