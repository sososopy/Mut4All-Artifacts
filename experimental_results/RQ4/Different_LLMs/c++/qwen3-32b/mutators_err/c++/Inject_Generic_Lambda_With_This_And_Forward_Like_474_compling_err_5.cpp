//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Generic_Lambda_With_This_And_Forward_Like_474
 */ 
class MutatorFrontendAction_474 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(474)

private:
    class MutatorASTConsumer_474 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_474(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::string variableName; // Store the variable name for the call
    };
};

//source file
#include "../include/Inject_Generic_Lambda_With_This_And_Forward_Like_474.h"

// ========================================================================================================
#define MUT474_OUTPUT 1

void MutatorFrontendAction_474::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        auto *LE = dyn_cast<clang::LambdaExpr>(VD->getInit());
        if (!LE)
            return;
        // Get the source code of the lambda
        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation start = LE->getBeginLoc();
        SourceLocation end = LE->getEndLoc();
        if (SM.isInMainFile(start)) {
            std::string lambdaCode = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(start, end).getAsRange());
            // Modify the lambda to add generic parameters and 'this'
            size_t capturePos = lambdaCode.find('[');
            if (capturePos != std::string::npos) {
                size_t closePos = lambdaCode.find(']');
                if (closePos != std::string::npos) {
                    std::string captureList = lambdaCode.substr(capturePos + 1, closePos - capturePos - 1);
                    std::string newLambda = lambdaCode;
                    newLambda.insert(closePos, "]<typename Self>(this Self&& self)");
                    // Find a captured variable to apply std::forward_like
                    if (!captureList.empty()) {
                        std::string varName = captureList;
                        size_t commaPos = varName.find(',');
                        if (commaPos != std::string::npos)
                            varName = varName.substr(0, commaPos);
                        varName.erase(std::remove(varName.begin(), varName.end(), ' '), varName.end());
                        // Replace the usage of the variable with std::forward_like<Self>(var)
                        size_t bodyStart = newLambda.find("{");
                        if (bodyStart != std::string::npos) {
                            std::string body = newLambda.substr(bodyStart);
                            size_t varPos = body.find(varName);
                            if (varPos != std::string::npos) {
                                body.replace(varPos, varName.length(), "std::forward_like<Self>(" + varName + ")");
                                newLambda.replace(bodyStart, body.length(), body);
                            }
                        }
                    }
                    // Replace the lambda code
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end).getAsRange(), newLambda);
                }
            }
            // Add #include <utility> if not present
            const FileEntry *FE = SM.getFileEntryForID(SM.getFileID(start));
            if (FE && FE->getName().endswith(".cpp")) {
                std::string includeLine = "#include <utility>\n";
                bool hasInclude = false;
                auto Buffer = SM.getBufferData(SM.getMainFileID());
                if (!Buffer.empty() && Buffer.find(includeLine) != std::string::npos) {
                    hasInclude = true;
                }
                if (!hasInclude) {
                    SourceLocation firstToken = SM.getLocForStartOfFile(SM.getMainFileID());
                    Rewrite.InsertText(firstToken, includeLine, true, true);
                }
            }
            variableName = VD->getNameAsString();
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;
        // Check if the call is to the variable we modified
        if (!variableName.empty()) {
            SourceLocation start = CE->getBeginLoc();
            SourceLocation end = CE->getEndLoc();
            if (Result.Context->getSourceManager().isWrittenInMainFile(start)) {
                std::string callCode = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(start, end).getAsRange());
                std::string newCall = "std::move(" + variableName + ")()";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end).getAsRange(), newCall);
            }
        }
    }
}

void MutatorFrontendAction_474::MutatorASTConsumer_474::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher varMatcher = varDecl(hasInitializer(lambdaExpr(hasCapture(capturesThis())).bind("lambda"))).bind("varDecl");
    StatementMatcher callMatcher = callExpr(callee(declRefExpr(to(varDecl())))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}