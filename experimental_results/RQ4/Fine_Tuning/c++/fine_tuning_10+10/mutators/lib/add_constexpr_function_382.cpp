//source file
#include "../include/Add_Constexpr_Function_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition())
            return;
        
        // Get the source code text of target node
        auto className = CL->getNameAsString();
        std::string constexprFunc = "/*mut382*/constexpr int getConstValue() const { return 42; }\n";
        
        // Perform mutation on the source code text by applying string replacement
        auto content = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
        if (content.rfind('}') != std::string::npos)
            content.insert(content.rfind('}'), constexprFunc);
        
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
        cur_classes.push_back(CL);
    }
}

void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}